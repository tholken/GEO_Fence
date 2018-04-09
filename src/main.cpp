#include <Arduino.h>

#ifndef GEO_CONFIG
  #include <config.h>
#endif
#define DEBUG_MODE


void setup() {
    // put your setup code here, to run once:
#ifdef DEBUG_MODE
    // Wait for the Serial Monitor Attaches.
    while (!Serial);
#endif

    delay(10000);
    // put your setup code here, to run once:
    // Start the Serial output so we can monitor
    Serial.begin(115200);
    Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());



   // Load the ISR Routine
   ISR_Button_Setup();

   // Load the SD Card
   //setupSD();

   // Load the GPS Unit.
   //setupGPS();

}

void loop() {
    // put your main code here, to run repeatedly:

  // Check to see if the Button was pressed and the ISR Caught it
  if(BUTTON_PRESSED) {

    Serial.println(F("READ THE: Button Pressed....\n"));



//
    /*
      printDateTime(GPS.date, GPS.time);
      printFloat(GPS.location.lat(), GPS.location.isValid(), 11, 6);
      printFloat(GPS.location.lng(), GPS.location.isValid(), 12, 6);
      
      printFloat(GPS.speed.mph(), GPS.speed.isValid(), 6, 2);
      printFloat(GPS.course.deg(), GPS.course.isValid(), 7, 2);
      printFloat(GPS.altitude.feet(), GPS.altitude.isValid(), 7, 2);
      printInt(GPS.satellites.value(), GPS.satellites.isValid(), 5);
    
      Serial.println();
    
    
    
      
      smartDelay(1000);
    
      // Panic if nothing from the GPS unit
      if (millis() > 5000 && GPS.charsProcessed() < 10)
        Serial.println("No GPS data received: check wiring");
    
    // */

//

    BUTTON_PRESSED = false;
    // Log that it got pressed
    //writeSD("Button Pressed\n");

    

   }

}