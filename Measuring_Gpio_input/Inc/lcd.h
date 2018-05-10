/**
  ******************************************************************************
  * @file       
  * @author
  * @version
  * @date
  * @brief
  *
  * @note
  *
  ******************************************************************************
  */
  
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f0xx_hal.h"

/* Exported define ----------------------------------------------------------*/
extern const char LCD_CHAR_BLANK;
extern const char LCD_CHAR_P;
extern const char LCD_CHAR_LEFT;
extern const char LCD_CHAR_RIGHT;
	 
/* Exported types ------------------------------------------------------------*/
	 
	 
typedef enum
{
	LCD_LINE_1 = 0,
	LCD_LINE_2 = 1,
	LCD_LINE_ALL,
	LCD_ALIGN_NO,
	LCD_ALIGN_LEFT,
	LCD_ALIGN_RIGHT,
	LCD_ALIGN_CENTER,
	LCD_MENIU_CURSOR_SET,
	LCD_MENIU_CURSOR_CLEAR,
	LCD_LINE_SIZE = 16, // 16 chars  + LINE END '\0'
	LCD_LINE_END = '\0'
}LCD_Config;

typedef enum
{
	LCD_CMD_CLEAR = 1,
	LCD_CMD_HOME = 2,
  LCD_FUNCTION_SET = 0x39,

/*LCD_FLOW_CONTROL = 107;
LCD_FOLOWER = 113;
LCD_ITERNAL_OSC = 23;
LCD_POWER = 89;
LCD_DISPLAY = 12;


	*/
}
   
   LCD_Cmd;

/* Exported constants --------------------------------------------------------*/
	 extern const uint8_t LCD_I2C_ADDRESS;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
	 void lcd_init(void);
   void lcd_xy(uint8_t x, LCD_Config y);
	 void lcd_put_char(char c);


   void lcd_put_string(const char *string);

	 extern char string_buffer[];
	 void lcd_put_chars(const char *string, char size);
	 
	 

	 

#ifdef __cplusplus
}
#endif

#endif /* __HEADER_H*/




/*****************************END OF FILE*************************************/

