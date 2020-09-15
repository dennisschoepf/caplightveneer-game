#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define SENSOR_PIN 8
#define LED_PIN 3
#define LED_COUNT 24
#define LED_LINE_COUNT 3
#define LED_PER_LINE_COUNT 8

void flashAllLEDs(uint32_t color);
void clearAllLEDs();
void lightLine(int lineIndex, uint32_t color);
void winAnimation();
void looseAnimation();
void startGame();

bool testRun = true;
bool isTouched = false;
bool gameStarted = false;
bool isUpwards = false;
int currentlyLightedRowIndex = 0;

/* Set up neopixel led strip as described in their documentation */
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

/* Indices of the light strip LEDs, first element in array marks the bottommost LED of each strip to be able to illuminate them in a line */
int stripIndexMatrix[3][8] = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {15, 14, 13, 12, 11, 10, 9, 8},
    {16, 17, 18, 19, 20, 21, 22, 23}};

void setup()
{
  Serial.begin(9600);

  /* Basic strip setup, adjust brightness as needed */
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  pinMode(SENSOR_PIN, INPUT);
  currentlyLightedRowIndex = LED_PER_LINE_COUNT - 1;
  gameStarted = true;
}

void loop()
{
  isTouched = digitalRead(SENSOR_PIN);

  if (gameStarted)
  {
    lightLine(currentlyLightedRowIndex, strip.Color(255, 0, 0));
    delay(300);

    Serial.print("Index: ");
    Serial.println(currentlyLightedRowIndex);
    Serial.print("Is Touched: ");
    Serial.println(isTouched);
    if (isTouched == 1 && currentlyLightedRowIndex < 3 && currentlyLightedRowIndex >= 0)
    {
      // Go up again
      isUpwards = true;
    }
    else if (currentlyLightedRowIndex <= 0)
    {
      // Game lost
      gameStarted = false;
      looseAnimation();
    }

    lightLine(currentlyLightedRowIndex, strip.Color(0, 0, 0));

    if (!isUpwards)
    {
      currentlyLightedRowIndex--;
    }
    else if (currentlyLightedRowIndex >= LED_PER_LINE_COUNT - 1)
    {
      isUpwards = false;
      currentlyLightedRowIndex--;
    }
    else
    {
      currentlyLightedRowIndex++;
    }
  }
}

void lightLinesUpwards(int rowIndex)
{
}

void winAnimation()
{
  for (int i = LED_PER_LINE_COUNT - 1; i >= 0; i--)
  {
    lightLine(i, strip.Color(0, 255, 0));
    delay(100);
  }
  delay(500);
  clearAllLEDs();
  delay(300);
  flashAllLEDs(strip.Color(0, 255, 0));
  delay(300);
  clearAllLEDs();
}

void looseAnimation()
{
  for (int i = 0; i < 2; i++)
  {
    flashAllLEDs(strip.Color(255, 0, 0));
    delay(300);
    clearAllLEDs();
    delay(300);
  }

  flashAllLEDs(strip.Color(255, 0, 0));
  delay(300);

  for (int i = LED_PER_LINE_COUNT; i > 0; i--)
  {
    lightLine(i - 1, strip.Color(0, 0, 0));
    delay(300 / i);
  }
}

void flashAllLEDs(uint32_t color)
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void clearAllLEDs()
{
  strip.clear();
  strip.show();
}

void lightLine(int lineIndex, uint32_t color)
{
  /* Illuminates all LEDs in a line depending on how the stripIndexMatrix is set up */
  for (int i = 0; i < LED_LINE_COUNT; i++)
  {
    strip.setPixelColor(stripIndexMatrix[i][lineIndex], color);
  }
  strip.show();
}