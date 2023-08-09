/***************************************************************************************/
/***************************************************************************************/
/**************************** Author : Mohammed Elsayaad *******************************/
/**************************** Date : 9 June,2023         *******************************/
/**************************** Describtion: DIO configuration file   ********************/
/***************************************************************************************/
/***************************************************************************************/


/************ Lib Includes ******************/
#include "StdTypes.h"
#include "Utiles.h"
/************ DIO Includes *****************/
#include "DIO_interface.h"

/************ LCD Driver Includes *************/
#include "LCD_interface.h"
#include "LCD_private.h"
#include "LCD_cnfig.h"


/*If serial communication with LCD is active then SPI is needed*/
#if LCD_SEND_MODE	== LCD_SERIAL
#include "SPI_interface.h"
#endif
/**************** End of Inclusion section *************************************/

/**************** Static functions implementations ****************************/

#if LCD_SEND_MODE	== LCD_PARALLEL

static void LCD_voidWriteCommand(u8 Copy_xCommand)
{
	DIO_voidWritePin(LCD_RS,LOW);
	DIO_voidWritePin(LCD_D7,READ_BIT(Copy_xCommand,7));
	DIO_voidWritePin(LCD_D6,READ_BIT(Copy_xCommand,6));
	DIO_voidWritePin(LCD_D5,READ_BIT(Copy_xCommand,5));
	DIO_voidWritePin(LCD_D4,READ_BIT(Copy_xCommand,4));	
	
	DIO_voidWritePin(LCD_EN,HIGH);
	_delay_ms(2);
	DIO_voidWritePin(LCD_EN,LOW);
	_delay_ms(2);
	DIO_voidWritePin(LCD_D7,READ_BIT(Copy_xCommand,3));
	DIO_voidWritePin(LCD_D6,READ_BIT(Copy_xCommand,2));
	DIO_voidWritePin(LCD_D5,READ_BIT(Copy_xCommand,1));
	DIO_voidWritePin(LCD_D4,READ_BIT(Copy_xCommand,0));
	DIO_voidWritePin(LCD_EN,HIGH);
	_delay_ms(2);
	DIO_voidWritePin(LCD_EN,LOW);
	_delay_ms(2);
	
}
static void LCD_voidWriteData(u8 Copy_xData)
{
	DIO_voidWritePin(LCD_RS,HIGH);
	DIO_voidWritePin(LCD_D7,READ_BIT(Copy_xData,7));
	DIO_voidWritePin(LCD_D6,READ_BIT(Copy_xData,6));
	DIO_voidWritePin(LCD_D5,READ_BIT(Copy_xData,5));
	DIO_voidWritePin(LCD_D4,READ_BIT(Copy_xData,4));
	
	DIO_voidWritePin(LCD_EN,HIGH);
	_delay_ms(2);
	DIO_voidWritePin(LCD_EN,LOW);
	_delay_ms(2);
	DIO_voidWritePin(LCD_D7,READ_BIT(Copy_xData,3));
	DIO_voidWritePin(LCD_D6,READ_BIT(Copy_xData,2));
	DIO_voidWritePin(LCD_D5,READ_BIT(Copy_xData,1));
	DIO_voidWritePin(LCD_D4,READ_BIT(Copy_xData,0));
	DIO_voidWritePin(LCD_EN,HIGH);
	_delay_ms(2);
	DIO_voidWritePin(LCD_EN,LOW);
	_delay_ms(2);
	
}
#elif LCD_SEND_MODE	== LCD_SERIAL

	static void LCD_voidWriteCommand(u8 Copy_xCommand)
	{
		u8 Local_u8SendData=0;
		DIO_voidWritePin(LCD_RS,LOW);
		Local_u8SendData = (((Copy_xCommand)& 0xf0)>>3); /*1111 0000*/
		SPI_u8SendReceive_Polling(Local_u8SendData);
		DIO_voidWritePin(LCD_EN,HIGH);
		_delay_ms(2);
		DIO_voidWritePin(LCD_EN,LOW);
		Local_u8SendData = (((Copy_xCommand)& 0x0f)<<1); /*1111 0000*/
		SPI_u8SendReceive_Polling(Local_u8SendData);
		DIO_voidWritePin(LCD_EN,HIGH);
		_delay_ms(2);
		DIO_voidWritePin(LCD_EN,LOW);
		_delay_ms(2);
	}
	
	static void LCD_voidWriteData(u8 Copy_xData)
	{
		u8 Local_u8SendData=0;
		DIO_voidWritePin(LCD_RS,HIGH);
		Local_u8SendData = (((Copy_xData)& 0xf0)>>3); /*1111 0000*/
		SPI_u8SendReceive_Polling(Local_u8SendData);
		DIO_voidWritePin(LCD_EN,HIGH);
		_delay_ms(2);
		DIO_voidWritePin(LCD_EN,LOW);
		Local_u8SendData = (((Copy_xData)& 0x0f)<<1); /*1111 0000*/
		SPI_u8SendReceive_Polling(Local_u8SendData);
		DIO_voidWritePin(LCD_EN,HIGH);
		_delay_ms(2);
		DIO_voidWritePin(LCD_EN,LOW);
		_delay_ms(2);
		
	}


#endif
/**************** External functions implementations ****************************/

void HAL_LCD_voidInit(void)
{
	#if LCD_SEND_MODE == LCD_SERIAL
	SPI_voidInit();
	#endif
	/*Set all pins to output*/
	DIO_voidSetPinDirection(LCD_D4,OUTPUT);
	DIO_voidSetPinDirection(LCD_D5,OUTPUT);
	DIO_voidSetPinDirection(LCD_D6,OUTPUT);
	DIO_voidSetPinDirection(LCD_D7,OUTPUT);
	DIO_voidSetPinDirection(LCD_RS,OUTPUT);
	DIO_voidSetPinDirection(LCD_EN,OUTPUT);
	/*Start Init algorithm*/
	_delay_ms(40);
	LCD_voidWriteCommand(_4_BITCOMMAND);
	LCD_voidWriteCommand(FUNCTION_SET);
	_delay_ms(2);
	LCD_voidWriteCommand(DISP_ON_OFF);
	_delay_ms(2);
	LCD_voidWriteCommand(DISP_CLEAR);
	_delay_ms(2);
	LCD_voidWriteCommand(ENTRY_MODE);
	_delay_ms(2);
}

void HAL_LCD_voidSendChar(u8 Copy_u8Char)
{
	LCD_voidWriteData(Copy_u8Char);
}
void HAL_LCD_voidSendString(const char *Copy_pu8String)
{
	u8 Local_u8Iterator=0;
	for(;Copy_pu8String[Local_u8Iterator];Local_u8Iterator++)
	{
		LCD_voidWriteData(Copy_pu8String[Local_u8Iterator]);
	}
}
void HAL_LCD_voidSendNumber(u32 Copy_u32Number)
{
	u8 Local_u8Array[20]={0};
	s8 Local_s8Iterator=0;
	if(0 == Copy_u32Number)
	{
		HAL_LCD_voidSendChar('0');
	}
	else
	{
		while(Copy_u32Number)
		{
			Local_u8Array[Local_s8Iterator] = (Copy_u32Number%10) + '0';
			Local_s8Iterator++;
			Copy_u32Number /= 10;
		}
		Local_s8Iterator--;
		while(Local_s8Iterator >=0)
		{
			LCD_voidWriteData(Local_u8Array[Local_s8Iterator]);
			Local_s8Iterator--;
		}
	}
	
}
void HAL_LCD_voidGoTo(LCD_Line_t Copy_u8Line,u8 Copy_u8Cell)
{
	switch(Copy_u8Line)
	{
	case LINE1:		LCD_voidWriteCommand(0x80 | Copy_u8Cell);
		break;
	case LINE2:		LCD_voidWriteCommand(0xC0 | Copy_u8Cell);
		break;
	case LINE3:		LCD_voidWriteCommand(0x94 | Copy_u8Cell);
		break;
	case LINE4:		LCD_voidWriteCommand(0xD4 | Copy_u8Cell);
		break;
	}
}

void HAL_LCD_voidClearLCD(void)
{
	LCD_voidWriteCommand(DISP_CLEAR);
	_delay_ms(2);	
}

