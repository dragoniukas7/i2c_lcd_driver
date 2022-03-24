#ifndef I2C_LCD
#define I2C_LCD
#include "i2clcd_main.h"
#endif

#ifndef CHAR_DEVICE
#define CHAR_DEVICE
#include "char_device.h"
#endif

#define En 0x04 // Enable bit
#define Rw 0x02 // Read/Write bit
#define Rs 0x01 // Register select bit

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;
static uint8_t _cols;
static uint8_t _rows;
static uint8_t _charsize;
static uint8_t _backlightval;

void pulseEnable(struct i2c_client *client, uint8_t _data);
void command(struct i2c_client *client, uint8_t value);
size_t write(struct i2c_client *client, uint8_t value);
void send(struct i2c_client *client, uint8_t value, uint8_t mode);
void setCursor(struct i2c_client *client, uint8_t col, uint8_t row);
void lcd_print(struct i2c_client *client, char *text);
void turn_off(struct i2c_client *client);
void write4bits(struct i2c_client *client, unsigned char value);
void expanderWrite(struct i2c_client *client, unsigned char _data);
void lcd_init(struct i2c_client *client);