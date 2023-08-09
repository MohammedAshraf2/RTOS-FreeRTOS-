/***************************************************************************************/
/***************************************************************************************/
/**************************** Author : Mohammed Elsayaad *******************************/
/**************************** Date : 9 June,2023         *******************************/
/**************************** Describtion: DIO configuration file   ********************/
/***************************************************************************************/
/***************************************************************************************/

#ifndef LCD_CNFIG_H_
#define LCD_CNFIG_H_


/**
*	Options:
			LCD_2_16
			LCD_4_20
			
*/
#define LCD_TYPE LCD_4_20

/**
*	Options:
			LCD_PARALLEL
			LCD_SERIAL
			
*/

#define  LCD_SEND_MODE	LCD_PARALLEL

/*
#define LCD_RS	DIO_PA3
#define LCD_EN	DIO_PA2


#define LCD_D4	DIO_PB0
#define LCD_D5	DIO_PB1
#define LCD_D6	DIO_PB2
#define LCD_D7	DIO_PB4
*/

#define LCD_RS	DIO_PA1
#define LCD_EN	DIO_PA2


#define LCD_D4	DIO_PA3
#define LCD_D5	DIO_PA4
#define LCD_D6	DIO_PA5
#define LCD_D7	DIO_PA6


#endif /* LCD_CNFIG_H_ */