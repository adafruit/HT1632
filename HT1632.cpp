#include <WProgram.h>
#include "HT1632.h"

HT1632LEDMatrix::HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1) {
  matrices = (HT1632 *)malloc(sizeof(HT1632));

  matrices[0] = HT1632(data, wr, cs1);
  matrixNum  = 1;
  _width = 24;
  _height = 16;
}

HT1632LEDMatrix::HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1, uint8_t cs2) {
  matrices = (HT1632 *)malloc(2 * sizeof(HT1632));

  matrices[0] = HT1632(data, wr, cs1);
  matrices[1] = HT1632(data, wr, cs2);
  matrixNum  = 2;
  _width = 48;
  _height = 16;
}


void HT1632LEDMatrix::setPixel(uint8_t x, uint8_t y) {
  if (y >= _height) return;
  if (x >= _width) return;

  uint8_t m;
  // figure out which matrix controller it is
  m = x / 24;
  x %= 24;

  uint16_t i;

  if (x < 8) {
    i = 7;
  } else if (x < 16) {
    i = 128 + 7;
  } else {
    i = 256 + 7;
  }
  i -= (x % 8);

  if (y < 8) {
    y *= 2;
  } else {
    y = (y-8) * 2 + 1;
  } 

  i += y * 8;

  matrices[m].setPixel(i);
}


uint8_t HT1632LEDMatrix::width() {
  return _width;
}

uint8_t HT1632LEDMatrix::height() {
  return _height;
}

void HT1632LEDMatrix::begin(uint8_t type) {
  for (uint8_t i=0; i<matrixNum; i++) {
    matrices[i].begin(type);
  }
}

void HT1632LEDMatrix::clearScreen() {
  for (uint8_t i=0; i<matrixNum; i++) {
    matrices[i].clearScreen();
  }
}

void HT1632LEDMatrix::fillScreen() {
  for (uint8_t i=0; i<matrixNum; i++) {
    matrices[i].fillScreen();
  }
}

void HT1632LEDMatrix::setBrightness(uint8_t b) {
  for (uint8_t i=0; i<matrixNum; i++) {
    matrices[i].setBrightness(b);
  }
}

void HT1632LEDMatrix::blink(boolean b) {
  for (uint8_t i=0; i<matrixNum; i++) {
    matrices[i].blink(b);
  }
}

void HT1632LEDMatrix::writeScreen() {
  for (uint8_t i=0; i<matrixNum; i++) {
    matrices[i].writeScreen();
  }
}

//////////////////////////////////////////////////////////////////////////


HT1632::HT1632(int8_t data, int8_t wr, int8_t cs, int8_t rd) {
  _data = data;
  _wr = wr;
  _cs = cs;
  _rd = rd;

  for (uint8_t i=0; i<48; i++) {
    ledmatrix[i] = 0;
  }
}

void HT1632::begin(uint8_t type) {
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  pinMode(_wr, OUTPUT);
  digitalWrite(_wr, HIGH);
  pinMode(_data, OUTPUT);
  
  if (_rd >= 0) {
    pinMode(_rd, OUTPUT);
    digitalWrite(_rd, HIGH);
  }

  sendcommand(HT1632_SYS_EN);
  sendcommand(HT1632_LED_ON);
  sendcommand(HT1632_BLINK_OFF);
  sendcommand(HT1632_MASTER_MODE);
  sendcommand(HT1632_INT_RC);
  sendcommand(type);
  sendcommand(HT1632_PWM_CONTROL | 0xF);
  
  WIDTH = 24;
  HEIGHT = 16;
}

void HT1632::setBrightness(uint8_t pwm) {
  if (pwm > 15) pwm = 15;
  sendcommand(HT1632_PWM_CONTROL | pwm);
}

void HT1632::blink(boolean blinky) {
  if (blinky) 
    sendcommand(HT1632_BLINK_ON);
  else
    sendcommand(HT1632_BLINK_OFF);
}

void HT1632::setPixel(uint16_t i) {
  ledmatrix[i/8] |= _BV(i%8); 
}

void HT1632::clrPixel(uint16_t i) {
  ledmatrix[i/8] &= ~_BV(i%8); 
}

void HT1632::dumpScreen() {
Serial.println("---------------------------------------");
  for (uint16_t i=0; i<(WIDTH*HEIGHT/8); i++) {
    Serial.print("0x");
    Serial.print(ledmatrix[i], HEX);
    Serial.print(" ");
    if (i % 3 == 2) Serial.println();
  }

  Serial.println("\n---------------------------------------");
}

void HT1632::writeScreen() {
  for (uint16_t i=0; i<(WIDTH*HEIGHT/8); i++) {
    writeRAM(2*i, ledmatrix[i] >> 4);
    writeRAM(2*i+1, ledmatrix[i] & 0xF);
  }
}


void HT1632::clearScreen() {
  for (uint8_t i=0; i<(WIDTH*HEIGHT/8); i++) {
    ledmatrix[i] = 0;
  }
  writeScreen();
}


void HT1632::writedata(uint16_t d, uint8_t bits) {
  //Serial.println(d, HEX);
  pinMode(_data, OUTPUT);
  for (uint8_t i=bits; i > 0; i--) {
    digitalWrite(_wr, LOW);
   //Serial.println(_BV(i-1), HEX);
   if (d & _BV(i-1)) {
     digitalWrite(_data, HIGH);
   } else {
     digitalWrite(_data, LOW);
   }
  digitalWrite(_wr, HIGH);
  }
  pinMode(_data, INPUT);
}




void HT1632::writeRAM(uint8_t addr, uint8_t data) {
  //Serial.print("Writing 0x"); Serial.print(data&0xF, HEX);
  //Serial.print(" to 0x"); Serial.println(addr & 0x7F, HEX);

  uint16_t d = HT1632_WRITE;
  d <<= 7;
  d |= addr & 0x7F;
  d <<= 4;
  d |= data & 0xF;
 
  digitalWrite(_cs, LOW);
  writedata(d, 14);
  digitalWrite(_cs, HIGH);
}


uint8_t HT1632::sendcommand(uint8_t cmd) {
  uint16_t data = 0;
  data = HT1632_COMMAND;
  data <<= 8;
  data |= cmd;
  data <<= 1;
  
  digitalWrite(_cs, LOW);
  writedata(data, 12);
  digitalWrite(_cs, HIGH);  
}


void HT1632::fillScreen() {
  for (uint8_t i=0; i<(WIDTH*HEIGHT/8); i++) {
    ledmatrix[i] = 0xFF;
  }
  writeScreen();
}
