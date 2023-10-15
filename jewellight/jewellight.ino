#include "Adafruit_FreeTouch.h"
#include <Adafruit_NeoPixel.h>

// Hardware
#define CAPTOUCH_PIN 0
#define NEOPIXEL_PIN 1
#define NUM_LEDS 7
#define NUM_MODES 3
#define NUM_PATTERNS 8

long tick = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRBW + NEO_KHZ800);

// set up capacitive touch button using the FreeTouch library
// Calibrating your capacitive touch sensitivity: Change this variable to something between your capacitive touch serial readouts for on and off
long oldState = 0;
int touchThreshold = 400;
long inputCounter = 0;
Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(CAPTOUCH_PIN, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

struct Mode {
  uint8_t ledGroup[NUM_LEDS]; // assignement of each led to one of 3 groups
  uint32_t pattern[NUM_PATTERNS][3]; // assume there are 3 groups and 4 states
  // byte patternLength; for now hardcoded
  uint8_t timeFactor;
};

Mode modes[NUM_MODES] = {
  {{0, 1, 1, 1, 2, 2, 2}, { // Fire fighter
    {0xff5511, 0xff2200, 0x000000}, 
    {0xff5511, 0x000000, 0x000000}, 
    {0xff5511, 0xff2200, 0x000000}, 
    {0xff5511, 0x000000, 0x000000},
    {0xff5511, 0x000000, 0xff2200}, 
    {0xff5511, 0x000000, 0x000000}, 
    {0xff5511, 0x000000, 0xff2200}, 
    {0xff5511, 0x000000, 0x000000}
    }, 1},
  {{0, 1, 2, 1, 2, 1, 2}, { // Alternating red
    {0xff5511, 0xff0000, 0x000000}, 
    {0xff5511, 0x000000, 0xff0000}, 
    {0x000000, 0xff0000, 0x000000}, 
    {0x000000, 0x000000, 0xff0000},
    {0xff5511, 0xff0000, 0x000000},
    {0xff5511, 0x000000, 0xff0000},
    {0x000000, 0xff0000, 0x000000},
    {0x000000, 0x000000, 0xff0000},
    }, 3},
  {{0, 0, 1, 1, 0, 2, 2}, { // Police car
    {0x2222ff, 0xff0000, 0x000000}, 
    {0xff2222, 0x000000, 0x0000ff}, 
    {0x2222ff, 0xff0000, 0x000000}, 
    {0xff2222, 0x000000, 0x0000ff},
    {0x0000ff, 0xff0000, 0x000000}, 
    {0xff0000, 0x000000, 0x0000ff}, 
    {0x0000ff, 0xff0000, 0x000000}, 
    {0xff0000, 0x000000, 0x0000ff}
    }, 1}
};

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
  delay(70);
  tick++;
}

void getInput()
{
  long newState = qt_1.measure();  
  if (newState > touchThreshold && oldState < touchThreshold)
  {
    delay(20);
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
  int style = inputCounter % NUM_MODES;
  Mode mode = modes[style];
  //Serial.printf("%d, %d, %d \n", style, hue, tick);

  int i = 0;
  for (i=0; i < NUM_LEDS; i++)
  {
    uint8_t group = mode.ledGroup[i];
    uint32_t color = mode.pattern[(tick/mode.timeFactor) % NUM_PATTERNS][group];
    //int groint deltaOffset = (i + tick) % NUM_LEDS;
    //color = pixels.ColorHSV(hue + hueDelta * deltaOffset, saturation, light + lightDelta * deltaOffset);
    pixels.setPixelColor(i, color);
    //Serial.printf("%d, %d, %d, %d, %d \n", style, hue, i, color, tick);
  }
  pixels.show();
}


