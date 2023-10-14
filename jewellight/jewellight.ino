#include "Adafruit_FreeTouch.h"
#include <Adafruit_NeoPixel.h>

// Hardware
#define CAPTOUCH_PIN 0  //capacitive touch pin
#define NEOPIXEL_PIN 1  //neopixel ring pin
#define NUM_LEDS    7  //how many pixels total

// set up capacitive touch button using the FreeTouch library
// Calibrating your capacitive touch sensitivity: Change this variable to something between your capacitive touch serial readouts for on and off
long oldState = 0;
int touch = 500;
Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(CAPTOUCH_PIN, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

/*
  

  Serial.print(qt_1.measure());
  Serial.write(' ');
  checkpress();   //check to see if the button's been pressed
  delay(20);
}

void checkpress() {

// Get current button state.
 
    long newState =  qt_1.measure();  
    Serial.println(qt_1.measure());
   if (newState > touch && oldState < touch) {
    // Short delay to debounce button.
    delay(500);
    // Check if button is still low after debounce.
    long newState =  qt_1.measure(); }
*/

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, NEOPIXEL_PIN);

uint8_t  mode   = 2, // Current animation effect
         offset = 0;
uint32_t color  = 0X00A4B3; // Starting color
uint32_t prevTime; 

void setup() {
  pixels.begin();
  pixels.setBrightness(40); // 1/3 brightness

if (! qt_1.begin())  
  {
    Serial.println("Failed to begin qt on pin A1");
  }

  prevTime = millis();
}

void loop() {
  uint8_t  i;
  uint32_t t;

  Serial.println(qt_1.measure());

  switch(mode) {
    
   case 0: //rainbow hold
    rainbowHold(20);
    delay(500);
    break;
    
   case 1: //rainbow cycle slow
    rainbowCycleslow(20);
    delay(50);
    break;
       
   case 2: //rainbow cycle fast 
    rainbowCycle(5);
    delay(50);
    break;
  }

  t = millis();
  if((t - prevTime) > 8000) {      // Every 8 seconds...
    mode++;                        // Next mode
    if(mode > 3) {                 // End of modes?
      mode = 0;                    // Start modes over
      color >>= 8;                 // Next color R->G->B
      if(!color) color = 0xB300A4; // Reset color
    }
    for(i=0; i<32; i++) pixels.setPixelColor(i, 0);
    prevTime = t;
    
  }

  
}
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t r, j;

  for(j=0; j<256*6; j++) { // 6 cycles of all colors on wheel
    for(r=0; r< pixels.numPixels(); r++) {
      pixels.setPixelColor(r, Wheel(((r * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
void rainbowCycleslow(uint8_t wait) {
  uint16_t r, j;

  for(j=0; j<256*3; j++) { // 3 cycles of all colors on wheel
    for(r=0; r< pixels.numPixels(); r++) {
      pixels.setPixelColor(r, Wheel(((r * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
void rainbowHold(uint8_t wait) {
  uint16_t r, j;

  for(j=0; j<256*1; j++) { // 3 cycles of all colors on wheel
    for(r=0; r< pixels.numPixels(); r++) {
      pixels.setPixelColor(r, Wheel(((r * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

