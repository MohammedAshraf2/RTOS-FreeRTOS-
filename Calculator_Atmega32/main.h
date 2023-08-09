

/*File Guard*/
#ifndef MAIN_H_
#define MAIN_H_
#include <stdint-gcc.h>
/****************************** UserDefined Datatype ***********************************/

typedef struct
{
	int8_t firstVal;
	int8_t Operator;
	int8_t secondVal;
}equationMSG_t;

typedef struct
{
	equationMSG_t MSG;
	uint8_t updateFlag;
	uint8_t completeFlag;
}keypadBuffer_t;

typedef struct
{
	equationMSG_t equation;
	uint8_t resultVal;
}equationResultMSG_t;

/****************************** Macros Definitions ***********************************/
#define LCD_Task_STACK_SIZE				(uint16_t)260
#define KEYPAD_Task_STACK_SIZE			(uint16_t)260
#define CALC_Task_STACK_SIZE			(uint16_t)100
#define TIMING_Task_STACK_SIZE			(uint16_t)260

/*4 is the highest*/
#define LCD_Task_PRIORITY				(uint8_t)1
#define CALC_Task_PRIORITY				(uint8_t)2
#define KEYPAD_Task_PRIORITY			(uint8_t)3
#define TIMING_Task_PRIORITY			(uint8_t)4

/*Queues lengths*/
#define KEY2CALC_QUEUE_LENGTH			(uint8_t)1
#define KEY2LCD_QUEUE_LENGTH			(uint8_t)1
#define CALC2LCD_QUEUE_LENGTH			(uint8_t)1
#define TIM2LCD_QUEUE_LENGTH			(uint8_t)1
/*Queues widths*/
#define KEY2CALC_QUEUE_WIDTH			(uint16_t)sizeof(equationMSG_t)
#define KEY2LCD_QUEUE_WIDTH				(uint16_t)sizeof(equationMSG_t)
#define CALC2LCD_QUEUE_WIDTH			(uint16_t)sizeof(equationResultMSG_t)
#define TIM2LCD_QUEUE_WIDTH				(uint8_t)10
/*Time macros*/
#define UNIT_TIME_BASE					(uint8_t)60
#define TIME_STRING_LENGTH				(uint8_t)10
/*equation no input sign*/
#define NO_INPUT_YET					(int8_t)-1
/*Periodicity of tasks*/
#define LCD_TASK_PERIODICITY			((uint16_t)45)
#define KEYPAD_TASK_PERIODICITY			((uint16_t)145)
#define CALC_TASK_PERIODICITY			((uint16_t)403)
#define TIME_TASK_PERIODICITY			((uint16_t)1000)

/*Keypad Macros*/
#define KEYPAD_SMALLEST_VALUE			(uint8_t)'0'
#define KEYPAD_LARGEST_VALUE			(uint8_t)'9'
#define KEYPAD_CALCULATE_VALUE			(uint8_t)'='
/****************************** Tasks Decleration ***********************************/

extern void KeyPad_vTask(void *Pv);
extern void Calc_vTask(void *Pv);
extern void LCD_vTask(void *Pv);
extern void Timer_vTask(void *Pv);

/****************************** Private Functions Decleration ***********************************/
/**
* @brief: function take time and gather it in string.
* @Param: Copy_u8Seconds	seconds value.
* @Param: Copy_u8Minu		Minutes value.
* @Param: Copy_u8Hours		Hours value.
* @Param: Copy_pu8String	string carry the current time value.
*/
static void Get_vTimeNow(uint8_t Copy_u8Seconds,uint8_t Copy_u8Min,uint8_t Copy_u8Hours,uint8_t *Copy_pu8String);
static uint8_t Check_vKeypadInput(uint8_t Copy_u8KeyValue,keypadBuffer_t *Copy_pxKeypadBuffer);
#endif /* MAIN_H_ */