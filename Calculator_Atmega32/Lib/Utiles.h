#ifndef UTILES_H_
#define UTILES_H_

#include "StdTypes.h"
/************************************************************************/
/* Macros with arguments to clear,set,toggle and read bits.			    */
/************************************************************************/
#define MASK ((unsigned char)0x01)
#define PORT_MASK	((u8)0xff)
#define SET_BIT(REG,BIT)		(REG = (REG) | (MASK<<BIT))
#define CLR_BIT(REG,BIT)		(REG = (REG) & ~(MASK<<BIT))
#define READ_BIT(REG,BIT)		((REG>>BIT)&(MASK))
#define TOGGLE_BIT(REG,BIT)		(REG = REG ^ (MASK<<BIT))
#define Write_Bit(REG,BIT,VAL)	(REG = (REG & ~(MASK<<BIT)) | (VAL<<BIT))





/************************************************************************/
/************************ Functions Deceleration ************************/
/************************************************************************/

void SET_BIT_FUNC(volatile unsigned char*const Reg,unsigned char Bit);
void CLEAR_BIT_FUNC(volatile unsigned char*const Reg,unsigned char Bit);
signed char READ_BIT_FUNC(volatile unsigned char Reg,unsigned char Bit);

#endif /* UTILES_H_ */