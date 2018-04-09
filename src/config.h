// Set up the Board
#ifndef GEO_CONFIG
  #define GEO_CONFIG
#endif

// Interrupt Pin INT0
#define ISR_PIN 3   //Adalogger uses Pin3 (INT0) --> push button (SPST-NO) --> Ground
// SD Card Select Pin
#define CS_PIN  4
//  Pin #8 green LED for your blinking pleasure
#define WRITE_GLED  8
// Pin for External LED
#define ISR_LED_pin 10   // Connected to pin 10 ---> resistor  (220 Ohm) --> +LED --> Ground
//  Pin #13  (LED_BUILTIN) red LED for general purpose blinking (OnBoardLED)
#define ERROR_RLED  LED_BUILTIN


// Include the ERROR Handeling
#ifndef BUTTON_HEADER
  #include <Button_ISR_Functions.h>
#endif
// Include the ERROR Handeling
#ifndef LOG_HEADER
  #include <Error_Handling.h>
#endif
// Include the GPS Header
#ifndef GPS_HEADER
  #include <GPS_Functions.h>
#endif
// Include the SD_Card Header
#ifndef SD_HEADER
  #include <SD_Card_Functions.h>
#endif


