#include <Arduino.h>

// Config the Board information
#ifndef GEO_CONFIG
  #include <config.h>
#endif
#ifndef LOG_HEADER
  #define LOG_HEADER
#endif

// Declare Variables
#define ledErrorPin  ERROR_RLED
// standby command & boot successful message
#define PMTK_STANDBY "$PMTK161,0*28\r\n"

// Declare our Functions
static void error(uint8_t errno);


// blink out for error code
static void error(uint8_t errno) {
  // Shut off GPS
  Serial1.print(PMTK_STANDBY);
  
  while (1) {
    uint8_t i;
    for (i = 0; i < errno; i++) {
      digitalWrite(ledErrorPin, HIGH);
      delay(100);
      digitalWrite(ledErrorPin, LOW);
      delay(100);
    }
    for (i = errno; i < 10; i++) {
      delay(200);
    }
  }
}

