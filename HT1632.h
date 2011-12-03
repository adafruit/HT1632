#if(ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#define HT1632_READ  0x6
#define HT1632_WRITE 0x5
#define HT1632_COMMAND 0x4

#define HT1632_SYS_DIS 0x00
#define HT1632_SYS_EN 0x01
#define HT1632_LED_OFF 0x02
#define HT1632_LED_ON 0x03
#define HT1632_BLINK_OFF 0x08
#define HT1632_BLINK_ON 0x09
#define HT1632_SLAVE_MODE 0x10
#define HT1632_MASTER_MODE 0x14
#define HT1632_INT_RC 0x18
#define HT1632_EXT_CLK 0x1C
#define HT1632_PWM_CONTROL 0xA0

#define HT1632_COMMON_8NMOS  0x20
#define HT1632_COMMON_16NMOS  0x24
#define HT1632_COMMON_8PMOS  0x28
#define HT1632_COMMON_16PMOS  0x2C

class HT1632 {

 public:
  HT1632(int8_t data, int8_t wr, int8_t cs, int8_t rd = -1);

  void begin(uint8_t type);
  
  void clrPixel(uint16_t i);
  void setPixel(uint16_t i);

  void blink(boolean state);
  void setBrightness(uint8_t pwm);

  void clearScreen();
  void fillScreen();
  void writeScreen();
  void dumpScreen();
  
 private:
  int8_t WIDTH, HEIGHT;
  int8_t _data, _cs, _wr, _rd;
  uint8_t ledmatrix[48];     // 16 * 24 / 8
  void sendcommand(uint8_t c);
  void writedata(uint16_t d, uint8_t bits);
  void writeRAM(uint8_t addr, uint8_t data);
};

class HT1632LEDMatrix : public Print {
 public:
  HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1);
  HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1, uint8_t cs2);
  HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1, 
		  uint8_t cs, uint8_t cs3);
  HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1, 
		  uint8_t cs2, uint8_t cs3, uint8_t cs4);

 void begin(uint8_t type);
 void clearScreen(void);
 void fillScreen(void);
 void blink(boolean b);
 void setBrightness(uint8_t brightness);
 void writeScreen();
 uint8_t width();
 uint8_t height();

  void clrPixel(uint8_t x, uint8_t y);
  void setPixel(uint8_t x, uint8_t y);
  void drawPixel(uint8_t x, uint8_t y, uint8_t color);

  void drawLine(int8_t x0, int8_t y0, int8_t x1, int8_t y1, uint8_t color);
  void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
  void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
  void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
  void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);

  // Printing
  void setCursor(uint8_t x, uint8_t y);
  void setTextSize(uint8_t s);
  void setTextColor(uint8_t c);
#if(ARDUINO >= 100)
  size_t write(uint8_t c);
#else
  void write(uint8_t c);
#endif
  void drawChar(uint8_t x, uint8_t y, char c, uint16_t color, uint8_t size);

  void drawBitmap(uint8_t x, uint8_t y, 
		  const uint8_t *bitmap, uint8_t w, uint8_t h,
		  uint8_t color);


 private:
  HT1632 *matrices;
  uint8_t matrixNum, _width, _height;
  uint8_t cursor_x, cursor_y, textsize, textcolor;
};
