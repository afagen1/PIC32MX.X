
#include "MT3339.h"

GPS_gpgga newGPGGA;

void GPS_Init(void) {

    NMEA_Enable = 1;
    NMEA_State = 0;
    NMEA_Flag = 0;
    Xfer_Buff_Full = 0;
    UART_1_Init(9600);
    nmea_index = 0;
}

void GPS_String_Handler(void) {
    if (rx_nmea == '\r' || rx_nmea == '\n') {
        // NMEA_Xfer_Buff[nmea_index] = '\0';
        NMEA_Xfer_Buff[nmea_index] = '\r';
        NMEA_Xfer_Buff[nmea_index + 1] = '\0';
        nmea_index = 0;
        NMEA_State = 0;
        Xfer_Buff_Full = 1;
        //Evaluate GPS Code here
        Xfer_String(NMEA_Xfer_Buff, NMEA_Buffer_1);
//        Send_String_U1(NMEA_Buffer_1);
        GPS_Parse_NMEA(NMEA_Buffer_1);
        sprintf(buffer_1, "new parse: %f", Compute_Delta_T());
        Send_String_U1(buffer_1);
        parse_GGA(NMEA_Buffer_1);
        sprintf(buffer_1, "Austin parse: %f", Compute_Delta_T());
        Send_String_U1(buffer_1);
//        sprintf(buffer_1,"time: %s", newGPGGA.time);
//        Send_String_U1(buffer_1);
        Print_GGA();
    } else {
        NMEA_Xfer_Buff[nmea_index] = rx_nmea;
        nmea_index++;
    }
    NMEA_Flag = 0;


}

char GPS_Parse_NMEA(char *nmea_ptr) {
    Start_Delta_T();
  // first look if we even have one
  if (nmea_ptr[strlen(nmea_ptr)-4] == '*') {
    uint16_t sum = (uint16_t)parseHex(nmea_ptr[strlen(nmea_ptr)-3]) * 16;
    sum += parseHex(nmea_ptr[strlen(nmea_ptr)-2]);
    uint8_t i;
    // check checksum
    for (i=2; i < (strlen(nmea_ptr)-4); i++) {
      sum ^= nmea_ptr[i];
    }
    if (sum != 0) {
      // bad checksum :(
      return -1;
    }
  }
  
  int32_t degree;
  long minutes;
  char degreebuff[10];
  /////////////////////////////////////////////////////////////////////////////
  //*******************************GPGGA************************************///
  /////////////////////////////////////////////////////////////////////////////
  
  // look for a few common sentences
  if (strstr(nmea_ptr, "$GPGGA")) {
    // found GGA
    char *p = nmea_ptr;
    // get time
    p = strchr(p, ',')+1;
    float timef = atof(p);
    uint32_t time = timef;
    hour = time / 10000;
    minute = (time % 10000) / 100;
    seconds = (time % 100);

    milliseconds = fmod(timef, 1.0) * 1000;

    // parse out latitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 2);
      p += 2;
      degreebuff[2] = '\0';
      degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      minutes = 50 * atol(degreebuff) / 3;
      latitude_fixed = degree + minutes;
      latitude = degree / 100000 + minutes * 0.000006F;
      latitudeDegrees = (latitude-100*(int)(latitude/100))/60.0;
      latitudeDegrees += (int)(latitude/100);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'S') latitudeDegrees *= -1.0;
      if (p[0] == 'N') lat = 'N';
      else if (p[0] == 'S') lat = 'S';
      else if (p[0] == ',') lat = 0;
      else return -1;
    }

    // parse out longitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 3);
      p += 3;
      degreebuff[3] = '\0';
      degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      minutes = 50 * atol(degreebuff) / 3;
      longitude_fixed = degree + minutes;
      longitude = degree / 100000 + minutes * 0.000006F;
      longitudeDegrees = (longitude-100*(int)(longitude/100))/60.0;
      longitudeDegrees += (int)(longitude/100);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'W') longitudeDegrees *= -1.0;
      if (p[0] == 'W') lon = 'W';
      else if (p[0] == 'E') lon = 'E';
      else if (p[0] == ',') lon = 0;
      else return -1;
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      fixquality = atoi(p);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      satellites = atoi(p);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      HDOP = atof(p);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      altitude = atof(p);
    }

    p = strchr(p, ',')+1;
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      geoidheight = atof(p);
    }
    return 1;
  }
  //////////////////////////////////////////////////////////////////////////////
  //*****************************GPRMC****************************************//
  //////////////////////////////////////////////////////////////////////////////
  if (strstr(nmea_ptr, "$GPRMC")) {
   // found RMC
    char *p = nmea_ptr;

    // get time
    p = strchr(p, ',')+1;
    float timef = atof(p);
    uint32_t time = timef;
    hour = time / 10000;
    minute = (time % 10000) / 100;
    seconds = (time % 100);

    milliseconds = fmod(timef, 1.0) * 1000;

    p = strchr(p, ',')+1;
    // Serial.println(p);
    if (p[0] == 'A')
      fix = 1;
    else if (p[0] == 'V')
      fix = -1;
    else
      return -1;

    // parse out latitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 2);
      p += 2;
      degreebuff[2] = '\0';
      long degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      long minutes = 50 * atol(degreebuff) / 3;
      latitude_fixed = degree + minutes;
      latitude = degree / 100000 + minutes * 0.000006F;
      latitudeDegrees = (latitude-100*(int)(latitude/100))/60.0;
      latitudeDegrees += (int)(latitude/100);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'S') latitudeDegrees *= -1.0;
      if (p[0] == 'N') lat = 'N';
      else if (p[0] == 'S') lat = 'S';
      else if (p[0] == ',') lat = 0;
      else return -1;
    }

    // parse out longitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 3);
      p += 3;
      degreebuff[3] = '\0';
      degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      minutes = 50 * atol(degreebuff) / 3;
      longitude_fixed = degree + minutes;
      longitude = degree / 100000 + minutes * 0.000006F;
      longitudeDegrees = (longitude-100*(int)(longitude/100))/60.0;
      longitudeDegrees += (int)(longitude/100);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'W') longitudeDegrees *= -1.0;
      if (p[0] == 'W') lon = 'W';
      else if (p[0] == 'E') lon = 'E';
      else if (p[0] == ',') lon = 0;
      else return -1;
    }
    // speed
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      speed = atof(p);
    }

    // angle
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      angle = atof(p);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      uint32_t fulldate = atof(p);
      day = fulldate / 10000;
      month = (fulldate % 10000) / 100;
      year = (fulldate % 100);
    }
    // we dont parse the remaining, yet!
    return 1;
  }

  return -1;

}
uint8_t parseHex(char c) {
    if (c < '0')
      return 0;
    if (c <= '9')
      return c - '0';
    if (c < 'A')
       return 0;
    if (c <= 'F')
       return (c - 'A')+10;
    // if (c > 'F')
    return 0;
}

void parse_GGA(char *NMEA_ptr){
    Start_Delta_T();
    int counter = 0;
    int i = 0;
    while (*NMEA_ptr != '*') {
        i = 0;
        while (*NMEA_ptr != ',') {
            NMEA_Buffer_2[i++] = *NMEA_ptr;
            NMEA_ptr++;
        }
        NMEA_ptr++;
        NMEA_Buffer_1[i] = '\0';
//        Send_String_U1(NMEA_Buffer_2);
        switch(counter){
            case 1: 
                i = 0;
                while(NMEA_Buffer_2[i] != '\0'){
                newGPGGA.time[i]       = NMEA_Buffer_2[i];
                
                i++;
                }
                break;
            case 2:
                newGPGGA.latitude   = atof(NMEA_Buffer_2);
                
                break;
            case 3:
                newGPGGA.latDir     = *NMEA_Buffer_2;
                
                break;
            case 4:
                newGPGGA.longitude  = atof(NMEA_Buffer_2);
                
                break;
            case 5:
                newGPGGA.longDir    = *NMEA_Buffer_2;
                
                break;
            case 6: 
                newGPGGA.fix        = atoi(NMEA_Buffer_2);
                
                break;
            case 7:
                newGPGGA.sats       = atoi(NMEA_Buffer_2);
                
                break;
            case 8:
                newGPGGA.dilution   = atof(NMEA_Buffer_2);
                break;
            case 9:
                newGPGGA.altitude   = atof(NMEA_Buffer_2);
                break;
            case 10:
                newGPGGA.unitAlt    = *NMEA_Buffer_2;
                break;
            case 11: 
                newGPGGA.geoidal    = atof(NMEA_Buffer_2);
                break;
            case 12:
                newGPGGA.unitGeo    = *NMEA_Buffer_2;
                break;
            case 13:
                newGPGGA.dataAge    = atof(NMEA_Buffer_2);
                break;
            case 14:
                newGPGGA.diffID     = atoi(NMEA_Buffer_2);
                break;
            default:
                break;
                
        }
        counter++;
        memset(NMEA_Buffer_2, '\0', sizeof(NMEA_Buffer_1));
    }
    
}

void Xfer_String(char *from_ptr, char *to_ptr) {

    char length = sizeof (NMEA_Xfer_Buff);
    int i = 0;
    while (i != length + 1) {
        *to_ptr = *from_ptr;
        to_ptr++;
        from_ptr++;
        i++;
    }

}

void Xfer_2D(char *from_ptr) {
    upper_index = 0;
    lower_index = 0;
    while (*from_ptr != '*') {
        while (*from_ptr != ',') {
//            NMEA_Buffer_2[upper_index][lower_index] = *from_ptr;
            from_ptr++;
            lower_index++;
        }
        upper_index++;
        lower_index = 0;
    }
}

void Print_GGA(void){
    sprintf(buffer_1,"\ntime: %s \n", newGPGGA.time);
    Send_String_U1(buffer_1); 
    sprintf(buffer_1,"\nlat: %.4f, %c \n", newGPGGA.latitude, newGPGGA.latDir);
    Send_String_U1(buffer_1);  
    sprintf(buffer_1,"\nlong: %.4f, %c \n", newGPGGA.longitude, newGPGGA.longDir);
    Send_String_U1(buffer_1);  
    sprintf(buffer_1,"\nfix: %d \n", newGPGGA.fix);
    Send_String_U1(buffer_1); 
    sprintf(buffer_1,"\nsats: %d \n", newGPGGA.sats);
    Send_String_U1(buffer_1); 
    sprintf(buffer_1,"\ndilution: %.2f \n", newGPGGA.dilution);
    Send_String_U1(buffer_1); 
    sprintf(buffer_1,"\nalt: %.2f, %c \n", newGPGGA.altitude, newGPGGA.unitAlt);
    Send_String_U1(buffer_1);
    sprintf(buffer_1,"\nGEO: %.2f, %c \n", newGPGGA.geoidal, newGPGGA.unitGeo);
    Send_String_U1(buffer_1); 
    sprintf(buffer_1,"\nage: %.3f \n", newGPGGA.dataAge);
    Send_String_U1(buffer_1); 
    sprintf(buffer_1,"\nDiffID: %d \n", newGPGGA.diffID);
    Send_String_U1(buffer_1); 
    
}