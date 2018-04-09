#include <Arduino.h>
#ifndef GEO_CONFIG
  #include <config.h>
#endif

#ifndef BUTTON_HEADER
  #define BUTTON_HEADER
#endif


// Variables for Button_Debounce
// Set the amount of time for a button press
  const long debouncing_time = 19;  //Debouncing Time in Milliseconds
// Hold what time it is so we know we were last triggered to do something....
  volatile unsigned long last_micros;
// Hold the Value so we can grab it in the loop
  volatile bool BUTTON_PRESSED = false;
// Setup the ISR Pin to use.
  const byte interruptPin = ISR_PIN;          //Adalogger uses Pin3 (INT0) --> push button (SPST-NO) --> Ground
//Variables for LEDs
  const byte ext_ISR_LED = ISR_LED_pin;      // Connected to pin 10 ---> resistor  (220 Ohm) --> +LED --> Ground

volatile int current;         // Current state of the button
                              // (LOW is pressed b/c i'm using the pullup resistors)
volatile long millis_held;    // How long the button was held (milliseconds)
volatile long secs_held;      // How long the button was held (seconds)
volatile long prev_secs_held; // How long the button was held in the previous check
volatile byte previous = HIGH;
volatile unsigned long firstTime; // how long since the button was first pressed 
const int inPin = ISR_PIN;  // the pin number for input (for me a push button)
const int ledPin = ISR_LED_pin; 


// Declare our own functions
static void debounceInterrupt();
void ISR_Button_Setup();

void ledblink(int, int, int);
static void debounceInterrupt2();

// Just a simple helper function to blink an led in various patterns
void ledblink(int times, int lengthms, int pinnum){
  for (int x=0; x<times;x++) {
    digitalWrite(pinnum, HIGH);
    delay (lengthms);
    digitalWrite(pinnum, LOW);
    delay(lengthms);
  }
}

void debounceInterrupt2(){

  current = digitalRead(inPin);

  // if the button state changes to pressed, remember the start time 
  if (current == LOW && previous == HIGH && (millis() - firstTime) > 200) {
    firstTime = millis();
  }

  millis_held = (millis() - firstTime);
  secs_held = millis_held / 1000;

  // This "if" statement is a basic debouncing tool, the button must be pushed for at least
  // 100 milliseconds in a row for it to be considered as a push.
  if (millis_held > 50) {

    if (current == LOW && secs_held > prev_secs_held) {
      ledblink(1, 50, ledPin); // Each second the button is held blink the indicator led
    }

    // check if the button was released since we last checked
    if (current == HIGH && previous == LOW) {
      // HERE YOU WOULD ADD VARIOUS ACTIONS AND TIMES FOR YOUR OWN CODE
      // <<===============================================================================>>
      // <<===============================================================================>>
      // Button pressed for less than 1 second, one long LED blink
      if (secs_held <= 0) {
        Serial.print("Held for Less than 1 Second");
        Serial.print("It Works!!! Seconds held: ");
        Serial.print(secs_held);
        Serial.print("   Milliseconds held: ");
        Serial.println(millis_held);
        ledblink(1,750,ledPin);
      }

      // If the button was held for 1-3 seconds blink LED 10 times
      if (secs_held >= 1 && secs_held < 3) {
        Serial.print("Held for 1-3 Seconds");
        Serial.print("It Works!!! Seconds held: ");
        Serial.print(secs_held);
        Serial.print("   Milliseconds held: ");
        Serial.println(millis_held);
        ledblink(10,200,ledPin); 
      }

      // Button held for more than 3 seconds, print out some info
      if (secs_held >= 3) {
        Serial.print("It Works!!! Seconds held: ");
        Serial.print(secs_held);
        Serial.print("   Milliseconds held: ");
        Serial.println(millis_held);
      }
      // <<===============================================================================>>
    }
  }

  previous = current;
  prev_secs_held = secs_held;

}


//Helper Functions
void debounceInterrupt(){
    //Button Debounce
    // Debounce the Interrupt
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
      //Actually Do Something here....
        //  Caution: It will pause your loop while its here... do it quick!
        // Like send output to the Serial port
        Serial.println(F("Button Pressed"));
        // Toggle the FLAG
        BUTTON_PRESSED ^= true;

        // TOGGLE the LED
        digitalWrite(ext_ISR_LED, !digitalRead(ext_ISR_LED));
        // Then get out
    last_micros = micros();   // Remember when we did it
  }
}

void ISR_Button_Setup()
{
    // This Called once from Setup()
    //set pins as outputs
    pinMode(ext_ISR_LED, OUTPUT);         // External LED (Green)
    pinMode(interruptPin, INPUT_PULLUP);  // enable the Pull up resistor for the Switch http://playground.arduino.cc/CommonTopics/PullUpDownResistor

    //attach an interrupt to a PIN
    attachInterrupt(digitalPinToInterrupt(interruptPin), debounceInterrupt2, RISING);
}

