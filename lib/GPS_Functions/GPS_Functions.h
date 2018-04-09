#include <Arduino.h>
#ifndef GEO_CONFIG
  #include <config.h>
#endif

#ifndef GPS_HEADER
  #define GPS_HEADER
#endif

// The TinyGPS++ object
#include <TinyGPS++.h>
TinyGPSPlus GPS;
  /*
    #define PMTK_SET_NMEA_UPDATE_1HZ "$PMTK220,1000*1F"
    #define PMTK_SET_NMEA_UPDATE_5HZ "$PMTK220,200*2C"
    #define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"
  */
// Set refresh rate to 1 Second
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F\r\n"
// Turn On WAAS
#define PMTK_ENABLE_WAAS "$PMTK301,2*2E\r\n"
// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
// request for updates on antenna status 
#define PGCMD_ANTENNA "$PGCMD,33,1*6C\r\n" 
// standby command & boot successful message
#define PMTK_STANDBY "$PMTK161,0*28\r\n"

// Declare our own functions
static void smartDelay(unsigned long ms);
static void printFloat(float val, bool valid, int len, int prec);
static void printInt(unsigned long val, bool valid, int len);
static void printDateTime(TinyGPSDate &d, TinyGPSTime &t);
static void printStr(const char *str, int len);
static double convertDegMinToDecDeg (float degMin);
void setupGPS();


void setupGPS()
{
  Serial1.begin(57600);  // LS20031 baud- 57600
  
  //  In the WAAS system the corrections are transmitted from another satellite, not a GPS satellite, and
  //  received by a WAAS equipped gps receiver directly without needing a beacon or similar receiver
  Serial1.print(PMTK_ENABLE_WAAS); // Turned on WAAS

  // 57600 is the default baud rate for LS20031 GPS
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  // the following Turned off all NEMA sentences except GGA and RMC
  Serial1.print(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  Serial1.print(PMTK_SET_NMEA_UPDATE_1HZ); // Set the update rate to 1Hz output

  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
  // Request updates on antenna status, comment out to keep quiet
  Serial1.print(PGCMD_ANTENNA);
  
  delay(1000);
}


// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      GPS.encode(Serial1.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print("********** ");
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  if (!t.isValid())
  {
    Serial.print("******** ");
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}



