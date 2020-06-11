#include "Adafruit_GFX.h"

#ifndef ADA_HT1632_H_
#define ADA_HT1632_H_

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define ADA_HT1632_READ 0x06    //!< Read data from the RAM
#define ADA_HT1632_WRITE 0x05   //!< Write data to the RAM
#define ADA_HT1632_COMMAND 0x04 //!< Send command

#define ADA_HT1632_SYS_DIS                                                     \
  0x00 //!< Stops system clock, turns off LED duty cycle generator, reducing
       //!< power usage
#define ADA_HT1632_SYS_EN 0x01    //!< Turn on system oscillator
#define ADA_HT1632_LED_OFF 0x02   //!< Turn off LED duty cycle generator
#define ADA_HT1632_LED_ON 0x03    //!< Turn on LED duty cycle generator
#define ADA_HT1632_BLINK_OFF 0x08 //!< Turn off blinking function
#define ADA_HT1632_BLINK_ON 0x09  //!< Turn on blinking function
#define ADA_HT1632_SLAVE_MODE                                                  \
  0x10 //!< Set slave mode and clock source from external clock
#define ADA_HT1632_MASTER_MODE                                                 \
  0x14 //!< Set master mode and clock source from on-chip RC oscillator
#define ADA_HT1632_INT_RC 0x18 //!< Use internal RC oscillator
#define ADA_HT1632_EXT_CLK                                                     \
  0x1C //!<  Set master mode and clock source from external clock
#define ADA_HT1632_PWM_CONTROL 0xA0 //!<  PWM brightness control

#define ADA_HT1632_COMMON_8NMOS                                                \
  0x20 //!< NMOS open drain output driver for commons
#define ADA_HT1632_COMMON_16NMOS                                               \
  0x24 //!< NMOS open drain output driver for commons
#define ADA_HT1632_COMMON_8PMOS                                                \
  0x28 //!< PMOS open drain output driver for commons
#define ADA_HT1632_COMMON_16PMOS                                               \
  0x2C //!< PMOS open drain output driver for commons

/*!
 * @brief Driver for the Adafruit HT1632 LED matrix driver
 */
class Adafruit_HT1632 {

public:
  /*!
   * @brief LED matrix object
   * @param data Serial data input or output with high-pull resistor
   * @param wr WRITE clock input with pull-high resistor.
   * @param cs Chip select
   * @param rd Read block input
   */
  Adafruit_HT1632(int8_t data, int8_t wr, int8_t cs, int8_t rd = -1);

  /*!
   * @brief Initialize driver
   * @param type Matrix type
   * @return Returns whether or not the initialization was successful
   */
  void begin(uint8_t type),
      /*!
       * @brief Clears a specific pixel
       * @param i Pixel to clear
       */
      clrPixel(uint16_t i),
      /*!
       * @brief Sets a specific pixel
       * @param i Pixel to set
       */
      setPixel(uint16_t i),
      /*!
       * @brief Turn display on with or without blinking
       * @param state Whether or not to blink when turning on the display
       */
      blink(boolean state),
      /*!
       * @brief Sets screen brightness
       * @param pwm Desired brightness
       */
      setBrightness(uint8_t pwm),
      /*! Clears the screen */
      clearScreen(),
      /*! Fills the screen */
      fillScreen(),
      /*! Writes to the screen */
      writeScreen(),
      /*! Dumps the screen */
      dumpScreen();

protected:
  /**
   * _data private serial data output
   * _cs private chip select input
   * _wr private WRITE clock input
   * _rd private READ clock input with high-pull resistor
   *@{
   */
  int8_t _data, _cs, _wr, _rd;
  /** @}*/
  uint8_t ledmatrix[24 * 16 / 8]; //!< LED matrix size
  /*!
   * @brief Sends command to HT1632
   * @param c Command to send
   */
  void sendcommand(uint8_t c),
      /*!
       * @brief Writes data to the driver
       * @param d Write address
       * @param bits Bits to write
       */
      writedata(uint16_t d, uint8_t bits),
      /*!
       * @brief Writes RAM to the driver
       * @param addr Address to write to
       * @param data Data to write
       */
      writeRAM(uint8_t addr, uint8_t data);
#ifdef __AVR__
  volatile uint8_t *dataport, *csport, *wrport, *datadir;
  uint8_t datamask, csmask, wrmask;
#endif
};

/*!
 * HT1632LEDMatrix main class
 */
class Adafruit_HT1632LEDMatrix : public Adafruit_GFX {
public:
  /*!
   * @brief LED matrix function for 1 matrix
   * @param data Serial data input or output with high-pull resistor
   * @param wr WRITE clock input with pull-high resistor.
   * @param cs1 Chip select input 1
   */
  Adafruit_HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1);
  /*!
   * @brief LED matrix function for 2 matrixes
   * @param data Serial data input or output with high-pull resistor
   * @param wr WRITE clock input with pull-high resistor.
   * @param cs1 Chip select input 1
   * @param cs2 Chip select input 2
   */
  Adafruit_HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1, uint8_t cs2);
  /*!
   * @brief LED matrix function for 3 matrixes
   * @param data Serial data input or output with high-pull resistor
   * @param wr WRITE clock input with pull-high resistor.
   * @param cs1 Chip select input 1
   * @param cs Chip select input 2
   * @param cs3 Chip select input 3
   */
  Adafruit_HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1, uint8_t cs,
                           uint8_t cs3);
  /*!
   * @brief LED matrix function for 4 matrixes
   * @param data Serial data input or output with high-pull resistor
   * @param wr WRITE clock input with pull-high resistor.
   * @param cs1 Chip select input 1
   * @param cs2 Chip select input 2
   * @param cs3 Chip select input 3
   * @param cs4 Chip select input 4
   */
  Adafruit_HT1632LEDMatrix(uint8_t data, uint8_t wr, uint8_t cs1, uint8_t cs2,
                           uint8_t cs3, uint8_t cs4);

  /*!
   * @brief Initialize driver
   * @param type Matrix type
   * @return Returns whether or not the initialization was successful
   */
  boolean begin(uint8_t type);
  /*!
   * @brief Clears the screen
   */
  void clearScreen(void),
      /*!
       * @brief Fills the screen
       */
      fillScreen(void),
      /*!
       * @brief Turn display on with or without blinking
       * @param b Whether or not to blink when turning on the display
       */
      blink(boolean b),
      /*!
       * @brief Sets screen brightness
       * @param brightness Desired brightness
       */
      setBrightness(uint8_t brightness),
      /*!
       * @brief Writes to the screen
       */
      writeScreen(),
      /*!
       * @brief Clears specified pixel
       * @param x X value of specified pixel
       * @param y Y value of specified pixel
       */
      clrPixel(uint8_t x, uint8_t y),
      /*!
       * @brief Turns on specified pixel
       * @param x X value of specified pixel
       * @param y Y value of specified pixel
       */
      setPixel(uint8_t x, uint8_t y),
      /*!
       * @brief Sets specified pixel with specified color
       * @param x X value of desired pixel
       * @param y Y value of desired pixel
       * @param color color to draw to pixel
       */
      drawPixel(int16_t x, int16_t y, uint16_t color);

protected:
  Adafruit_HT1632 *matrices; //!< Matrix pointer
  uint8_t matrixNum;         //!< The matrix's number
};

#endif /* Adafruit HT1632_H_ */
