#include "Adafruit_FreeTouch.h"
#include <Adafruit_NeoPixel.h>

// Hardware
#define CAPTOUCH_PIN 0
#define NEOPIXEL_PIN 1
#define NUM_LEDS 7
#define NUM_STYLES 3

long tick = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRBW + NEO_KHZ800);

// set up capacitive touch button using the FreeTouch library
// Calibrating your capacitive touch sensitivity: Change this variable to something between your capacitive touch serial readouts for on and off
long oldState = 0;
int touchThreshold = 400;
long inputCounter = 0;
Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(CAPTOUCH_PIN, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);


void setup() {
  pixels.begin();
  pixels.setBrightness(30); // 1/3 brightness

  if (!qt_1.begin())  
  {
    Serial.println("Failed to begin qt on pin A1");
  }
}

void loop() {
  getInput();
  makeLight();
  delay(200);
  tick++;
}

void getInput()
{
  long newState =  qt_1.measure();  
  if (newState > touchThreshold && oldState < touchThreshold)
  {
    delay(100);
    // Check if button is still low after debounce.
    newState = qt_1.measure();
    if (newState > touchThreshold)
    {
      inputCounter++;
    }
  }
  oldState = newState;
  //Serial.printf("%d, %d \n", newState, inputCounter);
}

void makeLight()
{
  int style = inputCounter % NUM_STYLES;
  int i = 0;
  uint32_t color = 0x000000;
  uint16_t hue;
  uint16_t hueDelta;
  uint8_t saturation;
  uint8_t light;
  uint8_t lightDelta;
/*
  switch (style)
  {
    case 0:
    hue = 0;
    saturation = 255;
    light = 255;
    hueDelta = 127;
    lightDelta = -10;
    break;
    
    case 1:
    hue = 24845;
    saturation = 255;
    light = 255;
    hueDelta = 2040;
    lightDelta = -5;
    break;
       
    case 2:
    hue = 43690;
    saturation = 60;
    light = 140;
    hueDelta = 1024;
    lightDelta = 3;
    break;
  }

  //Serial.printf("%d, %d, %d \n", style, hue, tick);
  for (i=0; i < NUM_LEDS; i++)
  {
    int deltaOffset = (i + tick) % NUM_LEDS;
    color = pixels.ColorHSV(hue + hueDelta * deltaOffset, saturation, light + lightDelta * deltaOffset);
    pixels.setPixelColor(i, color);
    Serial.printf("%d, %d, %d, %d, %d \n", style, hue, i, color, tick);
  }
  */
  switch (style)
  {
    case 0:
      for (i=0; i < NUM_LEDS; i++)
      {
        color = pixels.ColorHSV(i*8000, 255, 255);
        //color = pixels.Color(i*32, 0, 0);
        pixels.setPixelColor(i, color);
        //pixels.setPixelColor(i, 255, 0, 0);
        Serial.printf("%d, %d, %d \n", style, i, color);
      }    
    break;
    case 1:
      for (i=0; i < NUM_LEDS; i++)
      {
        color = pixels.ColorHSV(255, i*40, 255);
        //color = pixels.Color(0, i*32, 0);
        pixels.setPixelColor(i, color);
        //pixels.setPixelColor(i, 0, 255, 0);
        Serial.printf("%d, %d, %d \n", style, i, color);
      }    
    break;
    case 2:
      for (i=0; i < NUM_LEDS; i++)
      {
        color = pixels.ColorHSV(255, 255, i*40);
        //color = pixels.Color(0, 0, i*32);
        pixels.setPixelColor(i, color);
        //pixels.setPixelColor(i, 0, 0, 255);
        Serial.printf("%d, %d, %d \n", style, i, color);
      }    
    break;
  }
  pixels.show();
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

