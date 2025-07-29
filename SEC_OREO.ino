#include <Wire.h>
#include <Adafruit_MPR121.h>
#include <FastLED.h>


#define LED_PIN    2
#define NUM_LEDS   23
CRGB leds[NUM_LEDS];
uint8_t ledBrightness = 179;

#define SDA_PIN   0
#define SCL_PIN   1
Adafruit_MPR121 capTouch = Adafruit_MPR121();

// Outer (0–15) and Inner LEDs (16–22)
#define OUTER_COUNT 16
#define INNER_COUNT 7

// Row mapping for smooth vertical LED animations
const uint8_t rows[][5] = {
  {0, 1, 15, 255, 255},
  {2, 16, 14, 255, 255},
  {3, 17, 18, 13, 255},
  {4, 19, 12, 255, 255},
  {5, 20, 21, 11, 255},
  {6, 22, 10, 255, 255},
  {7, 8, 9, 255, 255}
};
constexpr uint8_t numRows = sizeof(rows) / sizeof(rows[0]);

typedef void (*AnimationFunc)(CRGB*, uint8_t);

// Track previous touch state
uint16_t lastTouchState = 0;


////////////////////////////////////////////////////
// Battery Charging Management
////////////////////////////////////////////////////
void disableBQ25895ILIM(uint8_t i2cAddress = 0x6A) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(0x00);  // REG00
  if (Wire.endTransmission(false) != 0) {
    Serial.println("BQ25895: Failed to initiate communication.");
    return;
  }

  Wire.requestFrom(i2cAddress, (uint8_t)1);
  if (!Wire.available()) {
    Serial.println("BQ25895: No data received.");
    return;
  }

  uint8_t reg0 = Wire.read();
  uint8_t newReg0 = reg0 & ~(1 << 5);  // Clear EN_ILIM bit

  Wire.beginTransmission(i2cAddress);
  Wire.write(0x00);
  Wire.write(newReg0);
  if (Wire.endTransmission() == 0) {
    Serial.println("BQ25895: ILIM resistor disabled successfully.");
  } else {
    Serial.println("BQ25895: Failed to write new value.");
  }
}

void setBQ25895ChargeCurrent(uint16_t mA, uint8_t i2cAddress = 0x6A) {
  if (mA < 0 || mA > 5056) {
    Serial.println("BQ25895: Invalid charge current.");
    return;
  }
  uint8_t value = mA / 64;  // Convert mA to register steps

  Wire.beginTransmission(i2cAddress);
  Wire.write(0x04);  // Charge current register
  Wire.endTransmission(false);

  Wire.requestFrom(i2cAddress, (uint8_t)1);
  if (!Wire.available()) {
    Serial.println("BQ25895: Failed to read charge current register.");
    return;
  }

  uint8_t reg = Wire.read();
  reg = (reg & 0xC0) | (value & 0x3F);  // Preserve bits [7:6], update [5:0]

  Wire.beginTransmission(i2cAddress);
  Wire.write(0x04);
  Wire.write(reg);
  if (Wire.endTransmission() == 0) {
    Serial.print("BQ25895: Charge current set to ");
    Serial.print(value * 64);
    Serial.println(" mA.");
  } else {
    Serial.println("BQ25895: Failed to set charge current.");
  }
}



////////////////////////////////////////////////////
// Animation functions
////////////////////////////////////////////////////
void secColors(CRGB* leds, uint8_t count) {
  static float offset = 0;
  const float scrollSpeed = 0.01f;  // scroll rate

  for (uint8_t row = 0; row < numRows; row++) {
    float phase = fmodf(offset + row * 0.2f, 2.0f);  // smooth gradient shift
    float t = (phase < 1.0f) ? phase : (2.0f - phase); // bounce between 0 → 1 → 0
    CRGB color = blend(CRGB::Blue, CRGB::Purple, uint8_t(t * 255));

    for (uint8_t i = 0; i < 5; i++) {
      uint8_t ledIdx = rows[row][i];
      if (ledIdx != 255 && ledIdx < count) {
        leds[ledIdx] = color;
      }
    }
  }
  offset += scrollSpeed;
  if (offset >= 2.0f) offset -= 2.0f;
}

void verticalRainbow(CRGB* leds, uint8_t count) {
  static uint8_t baseHue = 0;
  for (uint8_t row = 0; row < numRows; row++) {
    uint8_t rowHue = baseHue + row * 12;
    for (uint8_t i = 0; i < 5; i++) {
      uint8_t ledIdx = rows[row][i];
      if (ledIdx != 255 && ledIdx < count) {
        leds[ledIdx] = CHSV(rowHue, 255, 255);
      }
    }
  }
  baseHue++;
}

void partyAOuter(CRGB* leds, uint8_t count) {
  static uint8_t hue = 0;
  for (uint8_t i = 0; i < count; i++) {
    leds[i] = CHSV(hue + i * 16, 255, 255);
  }
  hue += 12; // fast change
}

void partyAInner(CRGB* leds, uint8_t count) {
  static uint8_t hue = 0;
  for (uint8_t i = 0; i < count; i++) {
    leds[i] = CHSV(hue + i * 32, 255, 255);
  }
  hue += 18; // fast change, different from outer
}

void partyBOuter(CRGB* leds, uint8_t count) {
  static uint8_t pulse = 0;
  static int8_t direction = 1;
  CRGB baseColor = CHSV(millis() / 8, 255, 255);

  for (uint8_t i = 0; i < count; i++) {
    uint8_t intensity = 128 + sin8(i * 20 + pulse) / 2;
    leds[i] = baseColor;
    leds[i].fadeLightBy(255 - intensity);
  }
  pulse += direction * 4;
}

void partyBInner(CRGB* leds, uint8_t count) {
  static unsigned long lastUpdate = 0;
  const unsigned long interval = 120; // slower speed
  unsigned long now = millis();
  if (now - lastUpdate < interval) return;
  lastUpdate = now;

  // Sample outer LED hue to invert it for contrast
  CRGB outerColor = leds[0];
  CHSV outerHSV = rgb2hsv_approximate(outerColor);
  uint8_t contrastHue = outerHSV.hue + 128;  // ~180° opposite hue

  for (uint8_t i = 0; i < count; i++) {
    leds[i] = CHSV(contrastHue + random8(-16, 16), 255, random8(180, 255));
  }
}

void partyCOuter(CRGB* leds, uint8_t count) {
  static uint8_t hueStep = 0;
  for (uint8_t i = 0; i < count; i++) {
    leds[i] = CHSV(hueStep + i * 32, 255, 255);
  }
  hueStep += 16;
}

void partyCInner(CRGB* leds, uint8_t count) {
  static uint8_t baseHue = 0;
  for (uint8_t i = 0; i < count; i++) {
    leds[i] = CHSV(baseHue + i * 8, 255, 255);
  }
  baseHue += 2;
}

// Macro for generating matrix effects
#define MATRIX_EFFECT(NAME, HUE) \
void NAME##Outer(CRGB* leds, uint8_t count) { \
  static uint8_t position = 0; \
  static unsigned long lastMove = 0; \
  const unsigned long delayMs = 150; \
  unsigned long now = millis(); \
  if (now - lastMove >= delayMs) { \
    lastMove = now; \
    position = (position == 0) ? numRows - 1 : position - 1; \
  } \
  for (uint8_t row = 0; row < numRows; row++) { \
    CRGB color = (HUE == 160 ? CHSV(HUE, 255, 255) : CHSV(HUE, 255, 200)); \
    if (row == position) color = CRGB::White; \
    for (uint8_t i = 0; i < 5; i++) { \
      uint8_t ledIdx = rows[row][i]; \
      if (ledIdx != 255 && ledIdx < count) { \
        leds[ledIdx] = color; \
      } \
    } \
  } \
} \
void NAME##Inner(CRGB* leds, uint8_t count) { \
  static int8_t rowIndex = numRows - 1; \
  static unsigned long lastMove = 0; \
  const unsigned long delayMs = 150; \
  unsigned long now = millis(); \
  if (now - lastMove >= delayMs) { \
    lastMove = now; \
    rowIndex = (rowIndex == 0) ? numRows - 1 : rowIndex - 1; \
  } \
  for (uint8_t row = 0; row < numRows; row++) { \
    CRGB color = CHSV(HUE, 255, 200); \
    if (row == rowIndex) color = CRGB::White; \
    for (uint8_t i = 0; i < 5; i++) { \
      uint8_t ledIdx = rows[row][i]; \
      if (ledIdx >= 16 && ledIdx < 23) { \
        leds[ledIdx - 16] = color; \
      } \
    } \
  } \
}

// Create variants
MATRIX_EFFECT(matrixGreen, 96)
MATRIX_EFFECT(matrixBlue, 160)
MATRIX_EFFECT(matrixRed, 0)


AnimationFunc outerAnimations[] = { partyBOuter, secColors, verticalRainbow, matrixGreenOuter, matrixBlueOuter, matrixRedOuter, partyAOuter, partyCOuter, partyCOuter };
AnimationFunc innerAnimations[] = { partyBInner, secColors, verticalRainbow, matrixGreenInner, matrixBlueInner, matrixRedInner, partyAInner, partyCInner, partyBInner };
uint8_t outerAnimIndex = 0;
uint8_t innerAnimIndex = 0;

unsigned long outerLastUpdate = 0;
unsigned long innerLastUpdate = 0;
const unsigned long frameInterval = 16; // ~60fps





////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////
void setup() {
  // Setup LEDs
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(ledBrightness);
  FastLED.show();

  // Setup I2C
  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);

  // Setup Capacitive Touch
  capTouch.begin(0x5A);  // Default I2C address

  // Setup battery charging
  disableBQ25895ILIM();
  setBQ25895ChargeCurrent(600);
}

void loop() {
  unsigned long now = millis();

  // Get current touch state
  uint16_t touchState = capTouch.touched();

  // Button A: animation change
  bool btn0Now = touchState & (1 << 0);
  bool btn0Prev = lastTouchState & (1 << 0);
  if (!btn0Prev && btn0Now) {
    outerAnimIndex = (outerAnimIndex + 1) % (sizeof(outerAnimations) / sizeof(AnimationFunc));
    innerAnimIndex = (innerAnimIndex + 1) % (sizeof(innerAnimations) / sizeof(AnimationFunc));
  }

  // Button B: brightness toggle
  bool btn1Now = touchState & (1 << 1);
  bool btn1Prev = lastTouchState & (1 << 1);
  if (!btn1Prev && btn1Now) {
    if (ledBrightness == 77) {
    ledBrightness = 179;  // ~70%
  } else if (ledBrightness == 179) {
    ledBrightness = 77;  // ~30%
  } else {
    ledBrightness = 179;  // fallback to 70%
  }
    FastLED.setBrightness(ledBrightness);
  }

  // Save state for next frame
  lastTouchState = touchState;

  // Run outer animation
  if (now - outerLastUpdate >= frameInterval) {
    outerLastUpdate = now;
    outerAnimations[outerAnimIndex](leds, OUTER_COUNT);
  }

  // Run inner animation
  if (now - innerLastUpdate >= frameInterval) {
    innerLastUpdate = now;
    innerAnimations[innerAnimIndex](leds + OUTER_COUNT, INNER_COUNT);
  }

  FastLED.show();
}