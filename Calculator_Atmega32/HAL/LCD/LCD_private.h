/***************************************************************************************/
/***************************************************************************************/
/**************************** Author : Mohammed Elsayaad *******************************/
/**************************** Date : 9 June,2023         *******************************/
/**************************** Describtion: DIO configuration file   ********************/
/***************************************************************************************/
/***************************************************************************************/

#ifndef LCD_PRIVATE_H_
#define LCD_PRIVATE_H_

/*make Lcd on and display cursor*/
#define DISP_ON_OFF		0x0c
/*Clear Display*/
#define DISP_CLEAR		0x01
/*make address of DDram increase by one and don't shift*/
#define ENTRY_MODE		0x06
/*Additional command needed to initialize 4 bit mode in lcd*/
#define _4_BITCOMMAND	0x02
/*for 4 bit mode initialization, 5*7 cell and 1 Line config*/
#define FUNCTION_SET	0x28

#define LCD_2_16	0
#define LCD_4_20	1

#define LCD_PARALLEL	2
#define LCD_SERIAL		1
static void LCD_voidWriteCommand(u8 Copy_xCommand);
static void LCD_voidWriteData(u8 Copy_xData);

#endif /* LCD_PRIVATE_H_ */