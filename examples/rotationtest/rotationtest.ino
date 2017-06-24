#include "Adafruit_GFX.h"
#include "Adafruit_HT1632.h"

#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5

// Use this line for single matrix
Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);
// Use this line for two matrices!
//Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS, HT_CS2);

void setup() {
  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);  
}

void loop() {
  testRotation(0); // This is the default
  testRotation(1);
  testRotation(2);
  testRotation(3);
  matrix.clearScreen();
}

void testRotation(uint8_t r) {
  matrix.clearScreen();

  // Print the current rotation then wait a bit
  matrix.setRotation(r);
  matrix.setCursor(0, 0);
  matrix.print(r);
  matrix.writeScreen();
  delay(500);

  matrix.clearScreen();

  // Light up each pixel starting from the top left to bottom right then wait a bit
  for (uint8_t y=0; y<matrix.height(); y++) {   // Every row
    for (uint8_t x=0; x< matrix.width(); x++) { // Every column
      matrix.setPixel(x, y);
      matrix.writeScreen();
    }
  }
  delay(500);
}
