#include "lcd_functions.h"

void lcd_init(struct i2c_client *client)
{
	_cols = 16;
	_rows = 2;
	_charsize = 8;
	_backlightval = LCD_BACKLIGHT;

	if (_rows > 1)
	{
		_displayfunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((_charsize != 0) && (_rows == 1))
	{
		_displayfunction |= LCD_5x10DOTS;
	}

	udelay(50);

	// Now we pull both RS and R/W low to begin commands
	expanderWrite(client, _backlightval); // reset expanderand turn backlight off (Bit 8 =1)
	udelay(1000);
    
	// put the LCD into 4 bit mode
	//  this is according to the hitachi HD44780 datasheet
	//  figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(client, 0x03 << 4);
	udelay(5); // wait min 4.1ms

	// second try
	write4bits(client, 0x03 << 4);
	udelay(5); // wait min 4.1ms

	// third go!
	write4bits(client, 0x03 << 4);
	udelay(2);

	write4bits(client, 0x02 << 4);
	udelay(2);

	// set # lines, font size, etc.
	command(client, LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF;
	command(client, LCD_DISPLAYCONTROL | _displaycontrol);
	udelay(10);

	// clear it off
	command(client, LCD_CLEARDISPLAY); // clear display, set cursor position to zero
	udelay(2000);			   // this command takes a long time!

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(client, LCD_ENTRYMODESET | _displaymode);

	udelay(100); // this command takes a long time!

	command(client, LCD_RETURNHOME); // set cursor position to zero
	udelay(200);			 // this command takes a long time!

	//lcd_print(client, "i2c lcd driver v0.1");
}

void lcd_print(struct i2c_client *client, char *text)
{
	int i;

	command(client, LCD_CLEARDISPLAY); // clear display, set cursor position to zero
	udelay(2000);
	
	setCursor(client, 0, 0);

	for (i = 0; i < strlen(text); i++)
	{
		if (i == 16)
		{
			setCursor(client, 0, 1);
		}
		else if(i == 32)
		break;

		write(client, text[i]);
	}
}

void setCursor(struct i2c_client *client, uint8_t col, uint8_t row)
{
	int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	if (row > _rows)
	{
		row = _rows - 1; // we count rows starting w/0
	}
	command(client, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void write4bits(struct i2c_client *client, unsigned char value)
{
	expanderWrite(client, value);
	pulseEnable(client, value);
}

void expanderWrite(struct i2c_client *client, unsigned char _data)
{
	unsigned char a;
	a = (int)_data | _backlightval;

	i2c_master_send(client, &a, 1);
	udelay(100);
}

void pulseEnable(struct i2c_client *client, uint8_t _data)
{
	unsigned char a;
	a = (_data | En);
	expanderWrite(client, a); // En high
	udelay(1);				   // enable pulse must be >450ns

	a = (_data & ~En);
	expanderWrite(client, a); // En low
	udelay(50);				   // commands need > 37us to settle
}

void command(struct i2c_client *client, uint8_t value)
{
	send(client, value, 0);
}

size_t write(struct i2c_client *client, uint8_t value)
{
	send(client, value, Rs);
	return 1;
}

void send(struct i2c_client *client, uint8_t value, uint8_t mode)
{
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	write4bits(client, (highnib) | mode);
	write4bits(client, (lownib) | mode);
}

void turn_off(struct i2c_client *client)
{
	_displaycontrol = LCD_DISPLAYOFF | LCD_CURSOROFF | LCD_BLINKOFF;
	command(client, LCD_DISPLAYCONTROL | _displaycontrol);
}