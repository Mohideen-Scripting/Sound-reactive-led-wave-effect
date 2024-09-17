#include <Adafruit_NeoPixel.h>

#define PIN            7     // Pin connected to the LED strip
#define NUM_LEDS       300    // Number of LEDs in the strip
#define CLAP_PIN       2     // Pin connected to the sound sensor
#define NUM_SETS       100     // Number of LED sets running
#define SET_SIZE       5     // Number of LEDs per set
#define DELAY_TIME     50    // Delay time between updates
#define BRIGHTNESS 255  // Adjust Brightness

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

volatile bool clapDetected = false;
int wavePositions[NUM_SETS] = {-SET_SIZE}; // Initialize off-screen
bool waveActive[NUM_SETS] = {false};       // Track if wave is active
int waveColor[NUM_SETS] = {0};
int color = 0;

void setup() {
  pinMode(CLAP_PIN, INPUT);
  strip.begin();
  strip.setBrightness(BRIGHTNESS); // Set the brightness to 50%
  strip.show();  // Initialize all pixels to 'off'
  attachInterrupt(digitalPinToInterrupt(CLAP_PIN), detectClap, RISING);
}

void loop() {
  if (clapDetected) {
    clapDetected = false;
    addWave();
  }

  runWaves();
  delay(DELAY_TIME); // Adjust speed of the wave effect
}

void detectClap() {
  clapDetected = true;
}

void update_color() {
  if (color == 0) {
    color += 1;
  }else if (color == 1) {
    color += 1;
  }else if (color == 2) {
    color -= 2;
  }
}

void addWave() {
  // Shift existing wave positions
  for (int i = NUM_SETS - 1; i > 0; --i) {
    wavePositions[i] = wavePositions[i - 1];
    waveActive[i] = waveActive[i - 1];
    waveColor[i] = waveColor[i-1];
  }
  // Add new wave at the beginning
  wavePositions[0] = 0;
  waveActive[0] = true;
  waveColor[0] = color;
  update_color();
}

void runWaves() {
  // Clear the strip first
  for (int i = 0; i < NUM_LEDS; ++i) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  // Update each wave
  for (int set = 0; set < NUM_SETS; ++set) {
    if (waveActive[set]) {
      int startPos = wavePositions[set];
      int endPos = startPos + SET_SIZE;

      for (int i = startPos; i < endPos; ++i) {
        if (i >= 0 && i < NUM_LEDS) {
          if (waveColor[set] == 0) {
            strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red color
          } else if (waveColor[set] == 1) {
            strip.setPixelColor(i, strip.Color(0, 255, 0)); // Green color
          } else if (waveColor[set] == 2) {
            strip.setPixelColor(i, strip.Color(0, 0, 255)); // blue color
          }
          
        }
      }

      // Move the wave position
      wavePositions[set]++;

      // Deactivate wave if it reaches the end
      if (wavePositions[set] > NUM_LEDS) {
        waveActive[set] = false;
      }
    }
  }

  strip.show();
}
