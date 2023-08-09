/***************************************************************************************/
/***************************************************************************************/
/**************************** Author : Mohammed Elsayaad *******************************/
/**************************** Date : 9 June,2023         *******************************/
/**************************** Describtion: DIO Private header file   *******************/
/***************************************************************************************/
/***************************************************************************************/

#ifndef DIO_PRIVATE_H_
#define DIO_PRIVATE_H_

#define PINA	(*((volatile u8 *)0x39))
#define DDRA	(*((volatile u8 *)0x3A))
#define PORTA	(*((volatile u8 *)0x3B))

#define PINB	(*((volatile u8 *)0x36))
#define DDRB	(*((volatile u8 *)0x37))
#define PORTB	(*((volatile u8 *)0x38))

#define PINC	(*((volatile u8 *)0x33))
#define DDRC	(*((volatile u8 *)0x34))
#define PORTC	(*((volatile u8 *)0x35))

#define PIND	(*((volatile u8 *)0x30))
#define DDRD	(*((volatile u8 *)0x31))
#define PORTD	(*((volatile u8 *)0x32))


extern PIN_TYPE DIO_Pin_Array[PIN_TOTAL_NUMBER];


#endif /* DIO_PRIVATE_H_ */