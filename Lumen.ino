#include <FastLED.h>

int startColourPicker();
void showSelectedColour(int savedHue);

// Prototype of Bike Lights project. It contains the following functionality:
// 1. Choose a colour.
// 2. Sleep mode after inactivity / wake mode after movement.
// 3. 'Smart braking' functionality - LEDs turn red when slowing down.

const int interruptPin = 0;
int colourInterrupt = 0;

const int led = 2;
int savedHue = 0;

const int buttonPin = 0;
int buttonState = 0;

// Variables related to the FastLED library:
#define LED_PIN     1
#define NUM_LEDS    50
#define BRIGHTNESS  22
#define LED_TYPE    WS2812B
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 10 // Controls the speed that the colour picker cycles through.

void setup() {
  // Set up the status LED and the interrupt pin.
  pinMode(led, OUTPUT);
  pinMode(interruptPin, INPUT);

  // Configure the LEDs and clear them before the program begins.
  FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  // Create the interrupt for the user to choose the LED colour.
  attachInterrupt(interruptPin, chooseColourTrigger, RISING);
}

void loop() {
  if(colourInterrupt == 1){
    digitalWrite(led, HIGH);
    savedHue = startColourPicker();
    showSelectedColour(savedHue);
    digitalWrite(led, LOW);

    // Reset the interrupt state:
    colourInterrupt = 0;
  }

}

void chooseColourTrigger(){
  // Set the intention ready for the next poll.
  colourInterrupt = 1;
}

int startColourPicker() {
    // Prevent the 'interrupt function' being interrupted.
    noInterrupts();
    int hue = 0;

    while(true){
         // Cycle through the possible colours, awaiting on user input.
         hue = hue + 1;
         fill_solid(leds, NUM_LEDS, CHSV(hue,255,255));
         FastLED.show();
                  
         // Check if the user has clicked the save button:
         buttonState = digitalRead(buttonPin);

         if(buttonState == HIGH){
            // Save the current colour as the selected colour and exit the loop.
            savedHue = hue;
            delay(200);
            return savedHue;
          }
          
         FastLED.delay(1000 / UPDATES_PER_SECOND);

        // Reset the colours back to red once it has cycled through HSV values.
        if(hue > 360) {
          hue = 0;
         }
    }
    
}

void showSelectedColour(int savedHue){
  // Show the colour that the user has selected.
  FastLED.clear();
  FastLED.show();
  delay(1000);
  fill_solid(leds, NUM_LEDS, CHSV(savedHue, 255, 255));
  FastLED.show();
  interrupts();
}
