


#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "CONFIG.h"
#include "EUSART.h"
#include "BNO055.h"

/**************************************************************************/
/**
 Different commands to set the update rate from once a second (1 Hz) to 10 times a second (10Hz)
 Note that these only control the rate at which the position is echoed, to actually speed up the
 position fix you must also send one of the position fix rate commands below too. */
#define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ  "$PMTK220,10000*2F"  ///< Once every 10 seconds, 100 millihertz.
#define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ  "$PMTK220,5000*1B"   ///< Once every 5 seconds, 200 millihertz.
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"              ///<  1 Hz
#define PMTK_SET_NMEA_UPDATE_2HZ  "$PMTK220,500*2B"               ///<  2 Hz
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"               ///<  5 Hz
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"               ///< 10 Hz
// Position fix update rate commands.
#define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ  "$PMTK300,10000,0,0,0,0*2C"  ///< Once every 10 seconds, 100 millihertz.
#define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ  "$PMTK300,5000,0,0,0,0*18"   ///< Once every 5 seconds, 200 millihertz.
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C"              ///< 1 Hz
#define PMTK_API_SET_FIX_CTL_5HZ  "$PMTK300,200,0,0,0,0*2F"               ///< 5 Hz
// Can't fix position faster than 5 times a second!

#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C" ///< 57600 bps
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"   ///<  9600 bps

#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"  ///< turn on only the second sentence (GPRMC)
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"   ///< turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"  ///< turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"      ///< turn off output

// to generate your own sentences, check out the MTK command datasheet and use a checksum calculator
// such as the awesome http://www.hhhh.org/wiml/proj/nmeaxor.html

#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22"          ///< Start logging data
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"            ///< Stop logging data
#define PMTK_LOCUS_STARTSTOPACK "$PMTK001,185,3*3C"   ///< Acknowledge the start or stop command
#define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38"         ///< Query the logging status
#define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22"        ///< Erase the log flash data
#define LOCUS_OVERLAP 0                               ///< If flash is full, log will overwrite old data with new logs
#define LOCUS_FULLSTOP 1                              ///< If flash is full, logging will stop

#define PMTK_ENABLE_SBAS "$PMTK313,1*2E"              ///< Enable search for SBAS satellite (only works with 1Hz output rate)
#define PMTK_ENABLE_WAAS "$PMTK301,2*2E"              ///< Use WAAS for DGPS correction data

#define PMTK_STANDBY "$PMTK161,0*28"              ///< standby command & boot successful message
#define PMTK_STANDBY_SUCCESS "$PMTK001,161,3*36"  ///< Not needed currently
#define PMTK_AWAKE "$PMTK010,002*2D"              ///< Wake up

#define PMTK_Q_RELEASE "$PMTK605*31"              ///< ask for the release and version


#define PGCMD_ANTENNA "$PGCMD,33,1*6C"            ///< request for updates on antenna status
#define PGCMD_NOANTENNA "$PGCMD,33,0*6D"          ///< don't show antenna status messages


char NMEA_State;
char NMEA_Enable;
char NMEA_Flag;
char Xfer_Buff_Full;
char rx_nmea;
int nmea_index;
int upper_index;
int lower_index;

uint8_t hour; ///< GMT hours
uint8_t minute; ///< GMT minutes
uint8_t seconds; ///< GMT seconds
uint16_t milliseconds; ///< GMT milliseconds
uint8_t year; ///< GMT year
uint8_t month; ///< GMT month
uint8_t day; ///< GMT day

//int8_t buffer_1[60];

float latitude; ///< Floating point latitude value in degrees/minutes as received from the GPS (DDMM.MMMM)
float longitude; ///< Floating point longitude value in degrees/minutes as received from the GPS (DDDMM.MMMM)

/** Fixed point latitude and longitude value with degrees stored in units of 1/100000 degrees,
  and minutes stored in units of 1/100000 degrees.  See pull #13 for more details:
  https://github.com/adafruit/Adafruit-GPS-Library/pull/13 */
int32_t latitude_fixed; ///< Fixed point latitude in decimal degrees
int32_t longitude_fixed; ///< Fixed point longitude in decimal degrees

float latitudeDegrees; ///< Latitude in decimal degrees
float longitudeDegrees; ///< Longitude in decimal degrees
float geoidheight; ///< Diff between geoid height and WGS84 height
float altitude; ///< Altitude in meters above MSL
float speed; ///< Current speed over ground in knots
float angle; ///< Course in degrees from true north
float magvariation; ///< Magnetic variation in degrees (vs. true north)
float HDOP; ///< Horizontal Dilution of Precision - relative accuracy of horizontal position
char lat; ///< N/S
char lon; ///< E/W
char mag; ///< Magnetic variation direction
char fix; ///< Have a fix?
uint8_t fixquality; ///< Fix quality (0, 1, 2 = Invalid, GPS, DGPS)
uint8_t satellites; ///< Number of satellites in use

char debug_buff[10];
char NMEA_Buffer_1 [79];
char NMEA_Buffer_2 [79];
char NMEA_Xfer_Buff[79];
/****GPS Structures************************************************/
typedef struct GPS_gprmcTag
{
    char    nav;            //navigation warning
    float   latitude;       //current latitude
    char    latDir;         //north/south
    float   longitude;      //current longitude
    char    longDir;        //east/west
    float   speed;          //speed knots
    int     course;         //True course    
    float   magVar;         //magnetic variation
//    char    date[];         //date of fix
    char    time[];         //time UTC
}GPS_gprmc;

typedef struct GPS_gpgllTag
{
    float   latitude;       //current latitude
    char    latDir;         //north/south
    float   longitude;      //current longitude
    char    longDir;        //east/west
}GPS_gpgll;

typedef struct GPS_gprmaTag
{
    char    status;         //status of data
    float   latitude;       //current latitude
    char    latDir;         //north or south latitude
    float   longitude;      //current longitude
    char    longDir;        //east or west longitude
    float   speed;          //speed over ground knots
    int     course;         //course over ground
    float   variation;      //variation 
    char    varDir;         //direction of variation east/west
}GPS_gprma;

typedef struct GPS_gpggaTag
{
    
    float   latitude;       //Latitude of position
    char    latDir;         //N/S
    float   longitude;      //longitude of position
    char    longDir;        //E/W
    int     fix;            //GPS Quality 0-no fix, 1-GPS, 2-DGPS
    int     sats;           //number of satellites
    float   dilution;       //horizontal dilution(relative accuracy of horizontal position)
    float   altitude;       //Antenna altitude above mean-sea-level
    char    unitAlt;        //units of altitude, meters
    float   geoidal;        //Geoidal separation
    char    unitGeo;        //units of Geoidal, meters
    float   dataAge;        //Age of DGPS data(seconds)
    int     diffID;         //Differential reference station ID
    char    time[];         //UTC of position
}GPS_gpgga;

/***END Structures*******************************************************************/

void GPS_Init(void);
void GPS_String_Handler(void);
char GPS_Parse_NMEA(char *NMEA_ptr);
uint8_t parseHex(char c);
void parse_GGA(char *NMEA_ptr);
void Xfer_String (char *from_ptr,char *to_ptr);
void Xfer_2D(char *from_ptr);
void Print_GGA(void);