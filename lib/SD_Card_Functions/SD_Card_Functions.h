#include <Arduino.h>

// Config the Board information
#ifndef GEO_CONFIG
  #include <config.h>
#endif
// Include the ERROR Handeling
#ifndef LOG_HEADER
  #include <Error_Handling.h>
#endif
// Include the GPS Header
#ifndef GPS_HEADER
  #include <GPS_Functions.h>
#endif

#ifndef SD_HEADER
  #define SD_HEADER
#endif
////////////////////////////////////////////////


//Includes for SD Card
#include <SPI.h>
#include <SD.h>
// Set the pins used
#define cardSelect CS_PIN
#define writeLED_Pin WRITE_GLED  //GREEN


#define GPS_LOG_FILE "GPS_FIX.txt"
#define LOGSPACE ", "

// Declare Functions
void setupSD();
void writeGPS_SD(TinyGPSDate, TinyGPSTime, TinyGPSLocation, TinyGPSSpeed, TinyGPSAltitude, const char );


// Functions
void setupSD()
{
  //set pins as outputs
  pinMode(writeLED_Pin, OUTPUT);         // GREEN LED Next to SD Card
  digitalWrite(writeLED_Pin, HIGH);
  Serial.print(F("Initializing SD card..."));
  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    digitalWrite(writeLED_Pin, LOW);
    Serial.println(F("SD Card failed, or not present"));
    // don't do anything more:
    error(10);
  }
  Serial.println(F("SD card initialized."));
  digitalWrite(writeLED_Pin, LOW);
}



void writeSD(const char *msg)
{
  digitalWrite(writeLED_Pin, HIGH);
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File logFile;
  logFile = SD.open(GPS_LOG_FILE, FILE_WRITE);
  digitalWrite(writeLED_Pin, LOW);
  // if the file opened okay, write to it:
  if (logFile) {
    digitalWrite(writeLED_Pin, HIGH);
    Serial.println("Writing GPS information to GPS_LOG_FILE...");
    // Print the Message if any
    uint8_t slen = strlen(msg);
    for (uint8_t i=0; i<slen; ++i)
      logFile.print(msg[i] ? msg[i] : ' ');
   
    logFile.print("\n");
    // close the file:
    logFile.close();
    Serial.println("done.");
    digitalWrite(writeLED_Pin, LOW);
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening GPS_LOG_FILE");
    error(4);
  }
}

void writeGPS_SD(TinyGPSDate &d, TinyGPSTime &t, TinyGPSLocation &loc, TinyGPSSpeed &speed, TinyGPSAltitude &alt, const char *msg)
{
  digitalWrite(writeLED_Pin, HIGH);
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File logFile;
  logFile = SD.open(GPS_LOG_FILE, FILE_WRITE);
  digitalWrite(writeLED_Pin, LOW);

  // if the file opened okay, write to it:
  if (logFile) {
    digitalWrite(writeLED_Pin, HIGH);

    Serial.println("Writing GPS information to GPS_LOG_FILE...");
    char tz[32];
    sprintf(tz, "%02d:%02d:%02d:%03d ", t.hour(), t.minute(), t.second(), t.centisecond() );
    logFile.print(tz);
    logFile.print(", ");

    char dz[32];
    sprintf(dz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    logFile.print(dz);
    logFile.print(LOGSPACE);

    logFile.print(loc.lng());
    logFile.print(LOGSPACE);
    
    logFile.print(loc.lat());
    logFile.print(LOGSPACE);

    logFile.print(speed.mph());
    logFile.print(LOGSPACE);

    logFile.print(alt.feet());
    logFile.print(LOGSPACE);

    
    // Print the Message if any
    uint8_t slen = strlen(msg);
    for (uint8_t i=0; i<slen; ++i)
       logFile.print(msg[i] ? msg[i] : ' ');

    logFile.print("\n");

    // close the file:
    logFile.close();
    Serial.println("done.");
    digitalWrite(writeLED_Pin, LOW);

  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening GPS_LOG_FILE");
    error(4);
  }

}

