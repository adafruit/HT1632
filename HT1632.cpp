#include <WProgram.h>
#include "HT1632.h"

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

void HT1632::clrPixel(uint8_t x, uint8_t y) {
  ledmatrix[(y*WIDTH+x) / 8] &= ~_BV(7 - (y*WIDTH+x) % 8);
}

void HT1632::setPixel(uint8_t x, uint8_t y) {
  ledmatrix[(y*WIDTH+x) / 8] |= _BV(7 - (y*WIDTH+x) % 8);
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
