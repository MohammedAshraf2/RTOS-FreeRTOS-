/***************************************************************************************/
/***************************************************************************************/
/**************************** Author : Mohammed Elsayaad *******************************/
/**************************** Date : 9 June,2023         *******************************/
/**************************** Description: DIO Functions implementation file   *********/
/***************************************************************************************/
/***************************************************************************************/
#include "StdTypes.h"
#include "Utiles.h"

#include "DIO_interface.h"
#include "DIO_private.h"




void DIO_voidInit(void)
{
	PIN_TYPE Local_u8Pin=DIO_PA0;
	for(;Local_u8Pin<PIN_TOTAL_NUMBER;Local_u8Pin++)
	{
		DIO_voidSetPinDirection(Local_u8Pin,DIO_Pin_Array[Local_u8Pin]);
	}
}
void DIO_voidSetPinDirection(PIN_TYPE Copy_xPin,MODE_TYPE Copy_xMode)
{
	PORT_TYPE Local_xPort = Copy_xPin / 8;
	Copy_xPin = Copy_xPin % 8;
	switch (Local_xPort)
	{
	case DIO_PORTA:
		switch(Copy_xMode)
		{
		case INFREE:
			CLR_BIT(DDRA,Copy_xPin);
			CLR_BIT(PORTA,Copy_xPin);
			break;
		case INPULL:
			CLR_BIT(DDRA,Copy_xPin);
			SET_BIT(PORTA,Copy_xPin);
			break;
		case OUTPUT:
			SET_BIT(DDRA,Copy_xPin);
			CLR_BIT(PORTA,Copy_xPin);
			break;
		}
		break;
	case DIO_PORTB:
		switch(Copy_xMode)
		{
			case INFREE:
			CLR_BIT(DDRB,Copy_xPin);
			CLR_BIT(PORTB,Copy_xPin);
			break;
			case INPULL:
			CLR_BIT(DDRB,Copy_xPin);
			SET_BIT(PORTB,Copy_xPin);
			break;
			case OUTPUT:
			SET_BIT(DDRB,Copy_xPin);
			CLR_BIT(PORTB,Copy_xPin);
			break;
		}
		break;
	case DIO_PORTC:
		switch(Copy_xMode)
		{
			case INFREE:
			CLR_BIT(DDRC,Copy_xPin);
			CLR_BIT(PORTC,Copy_xPin);
			break;
			case INPULL:
			CLR_BIT(DDRC,Copy_xPin);
			SET_BIT(PORTC,Copy_xPin);
			break;
			case OUTPUT:
			SET_BIT(DDRC,Copy_xPin);
			CLR_BIT(PORTC,Copy_xPin);
			break;
		}
		break;
	case DIO_PORTD:
		switch(Copy_xMode)
		{
			case INFREE:
			CLR_BIT(DDRD,Copy_xPin);
			CLR_BIT(PORTD,Copy_xPin);
			break;
			case INPULL:
			CLR_BIT(DDRD,Copy_xPin);
			SET_BIT(PORTD,Copy_xPin);
			break;
			case OUTPUT:
			SET_BIT(DDRD,Copy_xPin);
			CLR_BIT(PORTD,Copy_xPin);
			break;
		}
		break;	
	}
}
void DIO_voidWritePin(PIN_TYPE Copy_xPin, Logic_TYPE Copy_xLogic)
{
	PORT_TYPE Local_xPort = Copy_xPin /8;
	Copy_xPin = Copy_xPin %8;
	if(HIGH == Copy_xLogic)
	{
		switch(Local_xPort)
		{
		case DIO_PORTA:
			SET_BIT(PORTA,Copy_xPin);
			break;
		case DIO_PORTB:
			SET_BIT(PORTB,Copy_xPin);
			break;
		case DIO_PORTC:
			SET_BIT(PORTC,Copy_xPin);
			break;
		case DIO_PORTD:
			SET_BIT(PORTD,Copy_xPin);
			break;
		}
	}
	else if(LOW == Copy_xLogic)
	{
		switch(Local_xPort)
		{
		case DIO_PORTA:
			CLR_BIT(PORTA,Copy_xPin);
			break;
		case DIO_PORTB:
			CLR_BIT(PORTB,Copy_xPin);
			break;
		case DIO_PORTC:
			CLR_BIT(PORTC,Copy_xPin);
			break;
		case DIO_PORTD:
			CLR_BIT(PORTD,Copy_xPin);
			break;
		}
	}
	else
	{
		/*Invalid*/
	}
}
void DIO_voidTogglePin(PIN_TYPE Copy_xPin)
{
	PORT_TYPE Local_xPort = Copy_xPin /8;
	Copy_xPin = Copy_xPin % 8;
	switch(Local_xPort)
	{
		case DIO_PORTA:
		SET_BIT(PORTA,Copy_xPin);
		break;
		case DIO_PORTB:
		SET_BIT(PORTB,Copy_xPin);
		break;
		case DIO_PORTC:
		SET_BIT(PORTC,Copy_xPin);
		break;
		case DIO_PORTD:
		SET_BIT(PORTD,Copy_xPin);
		break;
	}
}
Logic_TYPE DIO_xReadPinStatus(PIN_TYPE Copy_xPin)
{
	Logic_TYPE Local_xLogic = HIGH;
	PORT_TYPE Local_xPort = Copy_xPin / 8;
	Copy_xPin = Copy_xPin %8;
	switch(Local_xPort)
	{
	case DIO_PORTA:
			Local_xLogic = READ_BIT(PINA,Copy_xPin);
			break;
	case DIO_PORTB:
			Local_xLogic = READ_BIT(PINB,Copy_xPin);
			break;
	case DIO_PORTC:
			Local_xLogic = READ_BIT(PINC,Copy_xPin);
			break;
	case DIO_PORTD:
			Local_xLogic = READ_BIT(PIND,Copy_xPin);
			break;	
	}
	return (Local_xLogic);
}

void DIO_voidWritePort(PORT_TYPE Copy_xPort, u8 Copy_u8Value)
{
	switch (Copy_xPort)
	{
	case DIO_PORTA: PORTA = Copy_u8Value;
		break;
	case DIO_PORTB: PORTB = Copy_u8Value;
		break;
	case DIO_PORTC: PORTC = Copy_u8Value;
		break;
	case DIO_PORTD: PORTD = Copy_u8Value;
		break;
	}	
}
