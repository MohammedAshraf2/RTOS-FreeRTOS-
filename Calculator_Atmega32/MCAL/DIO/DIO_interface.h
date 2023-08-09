/***************************************************************************************/
/***************************************************************************************/
/**************************** Author : Mohammed Elsayaad *******************************/
/**************************** Date : 9 June,2023         *******************************/
/**************************** Describtion: DIO Functions interface file   **************/
/***************************************************************************************/
/***************************************************************************************/

#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

#include "StdTypes.h"

typedef enum
{
	DIO_PORTA=0,
	DIO_PORTB,
	DIO_PORTC,
	DIO_PORTD
} PORT_TYPE ;

typedef enum
{
	LOW,
	HIGH
} Logic_TYPE;

typedef enum
{
	INFREE,
	INPULL,
	OUTPUT
} MODE_TYPE;
typedef enum 
{
	DIO_PA0=0,
	DIO_PA1,
	DIO_PA2,
	DIO_PA3,
	DIO_PA4,
	DIO_PA5,
	DIO_PA6,
	DIO_PA7,
	
	DIO_PB0,
	DIO_PB1,
	DIO_PB2,
	DIO_PB3,
	DIO_PB4,
	DIO_PB5,
	DIO_PB6,
	DIO_PB7,
	
	DIO_PC0,
	DIO_PC1,
	DIO_PC2,
	DIO_PC3,
	DIO_PC4,
	DIO_PC5,
	DIO_PC6,
	DIO_PC7,
	
	DIO_PD0,
	DIO_PD1,
	DIO_PD2,
	DIO_PD3,
	DIO_PD4,
	DIO_PD5,
	DIO_PD6,
	DIO_PD7,
	PIN_TOTAL_NUMBER
} PIN_TYPE;


extern void DIO_voidInit(void);
extern void DIO_voidSetPinDirection(PIN_TYPE Copy_xPin,MODE_TYPE Copy_xMode);
extern void DIO_voidWritePin(PIN_TYPE Copy_xPin, Logic_TYPE Copy_xLogic);
extern void DIO_voidTogglePin(PIN_TYPE Copy_xPin);
extern Logic_TYPE DIO_xReadPinStatus(PIN_TYPE Copy_xPin);

extern void DIO_voidWritePort(PORT_TYPE Copy_xPort, u8 Copy_u8Value);





#endif /* DIO_INTERFACE_H_ */