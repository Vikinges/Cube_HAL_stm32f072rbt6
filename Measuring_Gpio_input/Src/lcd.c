/*
  ******************************************************************************
  * @file  lcd.c
  * @author
  * @version
  * @date
  * @brief
  * @note
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/

#include "lcd.h"
#include "stdlib.h"
#include <string.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const char LCD_CHAR_BLANK = ' ';
const char LCD_CHAR_P	= 'P';
const char LCD_CHAR_LEFT = '<';
const char LCD_CHAR_RIGHT= '>';

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


const uint8_t LCD_I2C_ADDRESS = 0x7C; 
extern I2C_HandleTypeDef hi2c1;

char lcd_line1_buffer[17];
char lcd_line2_buffer[17];

char string_buffer[17] = "                \0";

//char lcd_buffer[LCD_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
void lcd_put_line(const char *buffer, LCD_Config line, LCD_Config align, LCD_Config meniu_cursor);
void lcd_xy(uint8_t x, LCD_Config y);

void lcd_cmd(unsigned char cmd)
{
	uint8_t data[2];
	data[0] = 0x00;
	data[1] = cmd;
	HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDRESS, data, 2, 10);
}

#define OSC_MASK (uint8_t) (1<<4)
#define OSC_F0 (uint8_t) (1<<0)
#define OSC_F1 (uint8_t) (1<<1)
#define OSC_F2 (uint8_t) (1<<2)
#define OSC_BS (uint8_t) (1<<3)

#define POWER_ICON_MASK (uint8_t)( (1<<6) | (1<<4) )
#define POWER_ICON_C4 (uint8_t) (1<<0)
#define POWER_ICON_C5 (uint8_t) (1<<1)
#define POWER_ICON_BON (uint8_t) (1<<2)
#define POWER_ICON_ION (uint8_t) (1<<3)

#define FOLOWER_MASK (uint8_t)( (1<<6) | (1<<5) )
#define FOLOWER_RAB0 (uint8_t) (1<<0)
#define FOLOWER_RAB1 (uint8_t) (1<<1)
#define FOLOWER_RAB2 (uint8_t) (1<<2)
#define FOLOWER_FON (uint8_t) (1<<3)

#define CONTRAST_MASK (uint8_t)( (1<<6) | (1<<5) | (1<<4) )
#define CONTRAST_C0 (uint8_t) (1<<0)
#define CONTRAST_C1 (uint8_t) (1<<1)
#define CONTRAST_C2 (uint8_t) (1<<2)
#define CONTRAST_C3 (uint8_t) (1<<3)


#define DISPLAY_MASK (uint8_t)(1<<3)
#define DISPLAY_BLINK (uint8_t) (1<<0)
#define DISPLAY_CURSOR (uint8_t) (1<<1)
#define DISPLAY_ON (uint8_t) (1<<2)

#define CMD_DISPLAY_CLEAR (uint8_t) (1<<0)

uint8_t cmd = 0;

// LCD ini sequence 1 Func set --> 2 Func set --> 3 OSC --> 4 Constrast 
// --> 5 Power_ICON --> 6 Follower --> delay --> 7 Display ON
void lcd_init(void)
{
	//1 Func set
	lcd_cmd(0x38); // Function set IS = 0
	HAL_Delay(2);
	
	//2 Func set 
	lcd_cmd(0x39); // Function set IS = 1
	HAL_Delay(2);
	
	//3 OSC
	cmd = 0;
	cmd = OSC_MASK | OSC_F0 | OSC_F1 | OSC_F2;
	lcd_cmd(cmd); //
	HAL_Delay(2);
	
	//4 Contrast set
	cmd = 0;
	cmd = CONTRAST_MASK ;
				//| CONTRAST_C0 
				//| CONTRAST_C1 
				//| CONTRAST_C2  
				//| CONTRAST_C3;
	lcd_cmd(cmd); 
	HAL_Delay(2);
	
	//5 Power_ICON
	cmd = 0;
	cmd = POWER_ICON_MASK 
				| POWER_ICON_ION 
				| POWER_ICON_BON
				| POWER_ICON_C4
				| POWER_ICON_C5;
				
	lcd_cmd(cmd); // 0x7F Contrast control 0x7F= max
	HAL_Delay(2);
	
	//6 Folower set
	cmd = 0;
	cmd = FOLOWER_MASK
				| FOLOWER_RAB0 
				| FOLOWER_RAB1
				//| FOLOWER_RAB2
				| FOLOWER_FON;

	lcd_cmd(cmd);
	HAL_Delay(250);
	
  //7 Display on
	cmd = 0;
	cmd = DISPLAY_MASK | DISPLAY_ON;
	lcd_cmd(cmd);
	HAL_Delay(2);
	
	lcd_cmd(CMD_DISPLAY_CLEAR ); //Display Clear
	HAL_Delay(2);
	
}

volatile char lcd_cursor_index = 0;
volatile char lcd_line_index = 0;
void lcd_put_char(char c)
{
	uint8_t data[2];
	data[0] = 0x40;
	data[1] = c;
	HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDRESS, data, 2, 100);
	lcd_cursor_index++;
}

void lcd_line_clear(LCD_Config line)
{
	/*
  if(line == LCD_LINE_ALL)
	{
		int i = 0;	
		while(i < LCD_LINE_SIZE)
		{
			lcd_line1_buffer[i] = ' ';
			lcd_line2_buffer[i] = ' ';
			i++;
		}
		lcd_line1_buffer[LCD_LINE_SIZE-1] = LCD_LINE_END;
		lcd_line2_buffer[LCD_LINE_SIZE-1] = LCD_LINE_END;
		
		lcd_put_line(lcd_line1_buffer, LCD_LINE_1, LCD_ALIGN_LEFT);
		lcd_put_line(lcd_line2_buffer, LCD_LINE_2, LCD_ALIGN_LEFT);
		
		lcd_xy(0, LCD_LINE_1);
		
	}
	
	if(line == LCD_LINE_1)
	{
		int i = 0;	
		while(i < LCD_LINE_SIZE)
		{
			lcd_line1_buffer[i] = ' ';
			i++;
		}
		lcd_line1_buffer[LCD_LINE_SIZE-1] = LCD_LINE_END;
		lcd_put_line(lcd_line1_buffer, LCD_LINE_1, LCD_ALIGN_LEFT);

		lcd_xy(0, LCD_LINE_1);
  }
	
	if(line == LCD_LINE_2)
	{
		int i = 0;	
		while(i < LCD_LINE_SIZE)
		{
			lcd_line2_buffer[i] = ' ';
			i++;
		}
		lcd_line2_buffer[LCD_LINE_SIZE-1] = LCD_LINE_END;
		lcd_put_line(lcd_line2_buffer, LCD_LINE_2, LCD_ALIGN_LEFT);
		lcd_xy(0, LCD_LINE_2);
  }
	*/
}

void lcd_put_string(const char *string)
{
	while(*string != LCD_LINE_END)
	{
		lcd_put_char(*string);
		string++;
		if(*string == LCD_LINE_END)
		{
			break;
		}
	}
}

void lcd_put_chars(const char *string, char size)
{
	char index = 0; 
	while(index < size)
	{
		lcd_put_char(*string);
		string++;
		index++;
	}
}

void lcd_xy(uint8_t x, LCD_Config y)
{
	 lcd_cursor_index = x;
   lcd_line_index = (char) y;
	 lcd_cmd(LCD_CMD_HOME);
   if(y == LCD_LINE_1){lcd_cmd(0x80 +x);}
   if(y == LCD_LINE_2){lcd_cmd(0xC0 |x);}
}
