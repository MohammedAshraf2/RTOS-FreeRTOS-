#include "DIO_Interface.h"
#include "Keypad_interface.h"
#include "Keypad_cnfg.h"






void HAL_Keypad_voidInit(void)
{
	/*init rows as output*/
	DIO_voidSetPinDirection(Keypad_Rows[0],OUTPUT);
	DIO_voidSetPinDirection(Keypad_Rows[1],OUTPUT);
	DIO_voidSetPinDirection(Keypad_Rows[2],OUTPUT);
	DIO_voidSetPinDirection(Keypad_Rows[3],OUTPUT);
	/*init cloums as input*/
	DIO_voidSetPinDirection(Keypad_Colum[0],INPULL);
	DIO_voidSetPinDirection(Keypad_Colum[1],INPULL);
	DIO_voidSetPinDirection(Keypad_Colum[2],INPULL);
	DIO_voidSetPinDirection(Keypad_Colum[3],INPULL);
	/*********/
	DIO_voidWritePin(Keypad_Rows[0],HIGH);
	DIO_voidWritePin(Keypad_Rows[1],HIGH);
	DIO_voidWritePin(Keypad_Rows[2],HIGH);
	DIO_voidWritePin(Keypad_Rows[3],HIGH);
}

u8   HAL_Keypad_u8GetKey(void)
{
	u8 Local_u8Key=KEYPAD_NO_KEY;
	u8 Local_u8Count1=0;
	u8 Local_u8Count2=0;
	
	for(;Local_u8Count1<KEYPAD_ROWS;Local_u8Count1++)
	{
		DIO_voidWritePin(Keypad_Rows[Local_u8Count1],LOW);
		for(Local_u8Count2=0;Local_u8Count2<KEYPAD_COLUM;Local_u8Count2++)
		{
			if(LOW == DIO_xReadPinStatus(Keypad_Colum[Local_u8Count2]))
			{
				/*Key was pressed*/
				Local_u8Key = Keypad_Array[Local_u8Count1][Local_u8Count2];
				while((LOW == DIO_xReadPinStatus(Keypad_Colum[Local_u8Count2])));
			}
		}
		DIO_voidWritePin(Keypad_Rows[Local_u8Count1],HIGH);
	}
	return (Local_u8Key);
}