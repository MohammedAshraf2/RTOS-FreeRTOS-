#include "StdTypes.h"
#include "Keypad_cnfg.h"
#include "DIO_Interface.h"


u8 Keypad_Array[KEYPAD_ROWS][KEYPAD_COLUM] =
{
	{'7','8','9','/'},
	{'4','5','6','*'},
	{'1','2','3','-'},
	{'.','0','=','+'}
};
/*
u8 Keypad_Rows[KEYPAD_ROWS]   = {DIO_PC5,DIO_PC4,DIO_PC3,DIO_PC2};
u8 Keypad_Colum[KEYPAD_COLUM] = {DIO_PD7,DIO_PD6,DIO_PD5,DIO_PD3};
*/
u8 Keypad_Rows[KEYPAD_ROWS]   = {DIO_PB4,DIO_PB5,DIO_PB6,DIO_PB7};
u8 Keypad_Colum[KEYPAD_COLUM] = {DIO_PD2,DIO_PD3,DIO_PD4,DIO_PD5};