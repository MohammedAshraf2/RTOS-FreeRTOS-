/***************************************************************************************/
/***************************************************************************************/
/**************************** Author : Mohammed Elsayaad *******************************/
/**************************** Date : 9 June,2023         *******************************/
/**************************** Describtion: DIO configuration file   ********************/
/***************************************************************************************/
/***************************************************************************************/

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_
#include "StdTypes.h"


#if LCD_TYPE == LCD_4_20

typedef enum
{
	LINE1,
	LINE2,
	LINE3,
	LINE4
}LCD_Line_t;
#else
typedef enum
{
	LINE1,
	LINE2
}LCD_Line_t;
#endif

extern void HAL_LCD_voidInit(void);
extern void HAL_LCD_voidSendChar(u8 Copy_u8Char);
extern void HAL_LCD_voidSendString(const char *Copy_pu8String);
extern void HAL_LCD_voidSendNumber(u32 Copy_u32Number);
extern void HAL_LCD_voidGoTo(LCD_Line_t Copy_u8Line,u8 Copy_u8Cell);
extern void HAL_LCD_voidClearLCD(void);

#endif /* LCD_INTERFACE_H_ */