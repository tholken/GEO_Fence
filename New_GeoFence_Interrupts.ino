// Test code forGPS modules
/*
    Use the EEPROM library to hold the Ploygon Locations.
    https://www.arduino.cc/en/Reference/EEPROM

*/
//#include <avr/pgmspace.h>
#include <math.h>


#include <Adafruit_GPS.h>
Adafruit_GPS GPS(&Serial1);

//Includes for SD Card
#include <SPI.h>
#include <SD.h>
// Set the pins used
#define cardSelect 4
#define cardDetect 7
#define buttonPin 10
#define ledGreenPin  8
#define ledRedPin  13
#define enPin  8

#define pin  10;
#define ipin 3;  //Pin3 on the Feather is INT0
volatile int state = LOW;

long debouncing_time = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

//const boolean debug = false;
#define debug  false

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

//
// Helper functions------------------------
//
// converts lat/long from degree-minute format to decimal-degrees
double convertDegMinToDecDeg (float degMin) {
  double min = 0.0;
  double decDeg = 0.0;
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
  return decDeg;
}

// blink out for error code
void error(uint8_t errno) {
  // Shut off GPS
  pinMode(enPin, OUTPUT);
  while (1) {
    uint8_t i;
    for (i = 0; i < errno; i++) {
      digitalWrite(ledRedPin, HIGH);
      delay(100);
      digitalWrite(ledRedPin, LOW);
      delay(100);
    }
    for (i = errno; i < 10; i++) {
      delay(200);
    }
  }
}

void setupGPS()
{
  /*
    #define PMTK_SET_NMEA_UPDATE_1HZ "$PMTK220,1000*1F"
    #define PMTK_SET_NMEA_UPDATE_5HZ "$PMTK220,200*2C"
    #define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"
  */
  Serial.println(F("GPS library test!"));
  // 57600 is the default baud rate for LS20031 GPS
  GPS.begin(57600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data (NO Altitude)
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  // Ask for firmware version
  Serial1.println(PMTK_Q_RELEASE);
}


void setupSD()
{
  Serial.print(F("Initializing SD card..."));
  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println(F("SD Card failed, or not present"));
    // don't do anything more:
    error(2);
  }
  Serial.println(F("SD card initialized."));
}



void writeSD(uint8_t &hour, uint8_t &minute, uint8_t &seconds, uint16_t &milliseconds,
             uint8_t &day, uint8_t &month, uint8_t &year,
             float &latitude, char &lat, float &longitude, char &lon,
             float &speed, float &angle, float &alt, uint8_t &sat)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File logFile;
  logFile = SD.open("GPS_FIX.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (logFile) {
    Serial.print(F("Writing GPS information to GPS_FIX.txt..."));
    logFile.print(hour); logFile.print(F(":")); logFile.print(minute); logFile.print(F(":")); logFile.print(seconds); logFile.print(F(".")); logFile.print(milliseconds);
    logFile.print(F(","));
    logFile.print(month); logFile.print(F("/")); logFile.print(day); logFile.print(F("/")); logFile.print(year);
    logFile.print(F(","));
    logFile.print(convertDegMinToDecDeg(latitude), 6); logFile.print(F(" ")); logFile.print(lat);
    
     
    
    logFile.print(F(" ")); logFile.print(lat);
    logFile.print(F(","));
    logFile.print(convertDegMinToDecDeg(longitude), 6); logFile.print(F(" ")); logFile.print(lon);
    logFile.print(F(","));
    logFile.print(speed);
    logFile.print(F(","));
    logFile.print(angle);
    logFile.print(F(","));
    logFile.print(alt);
    logFile.print(F(", "));
    logFile.print(sat);

    logFile.print(F(""));
    logFile.println(F("\n"));

    // close the file:
    logFile.close();
    Serial.println(F("done."));
  } else {
    // if the file didn't open, print an error:
    Serial.println(F("error opening GPS_FIX.txt"));
    error(4);
  }

}

void setup() {
  // put your setup code here, to run once:
 // initialize the LED pin as an output:
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);


  setupGPS();
  setupSD();

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

}
