/**************************** Author : Mohammed Elsayaad *******************************/
/**************************** Date : 9 Aug,2023         *******************************/
/**************************** Description: Calculator using FreeRTOS  *******************************/
/*
The calculator project is a real-time application that uses a microcontroller (ATMega32), 
a keypad and an LCD to perform arithmetic operations.
The project uses FreeRTOS, a real-time operating system (RTOS) that manages the scheduling and
 synchronization of the tasks and queues.
The project has four tasks and four queues as follows:

KEYPAD_Task: This task reads the input from the keypad and sends it to a queue. It also debounces the keypad to avoid false readings.
The input can be digits, operators, or special keys such as equal.

LCD_Task   : This task displays the input and output of the calculator on the LCD.
It receives the display structure from a queue and updates the LCD accordingly.
The display structure contains the operands, the operator, the result, and the error flag of the calculator.

CALC_Task  : This task performs the arithmetic operation based on the input from the keypad and the switch.
It receives the input from two queues and sends the output to two queues.
It also handles any errors such as division by zero.

Timing_Task: This task handle the timing of the system as LCD should always display time during working as
follows (h:mm:ss).

The project demonstrates how RTOS can be used to create a responsive and reliable calculator application
using multiple tasks and queues.
*/ 
/***************************************************************************************/
/***************************************************************************************/

/************************************ Inclusion section start **********************************************/

/************* App Layer inclusion *******************/

#include "main.h"

/************* Service layer inclusions *******************/

#include "FreeRTOS.h"
#include "projdefs.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/************* HAL layer inclusions *******************/

#include "LCD_interface.h"
#include "Keypad_interface.h"
#include "UART_interface.h"
/************************************ Inclusion section end **********************************************/


/****************************** Global Variables Defination Start  ***********************************/

/*********Create All Needed Queues Handles************/

static QueueHandle_t	Key2Calc_Queue;
static QueueHandle_t	Key2LCD_Queue;
static QueueHandle_t	Time2LCD_Queue;
static QueueHandle_t	Calc2LCD_Queue;

/*********Create All Needed Semaphores Handles************/

static SemaphoreHandle_t KeyPad2LCD_ClearNotification;
static SemaphoreHandle_t KeyPad2Timer_StartCounting;
static SemaphoreHandle_t KeyPad2Timer_StopCounting;
static SemaphoreHandle_t Timer2KeyPad_TenSecondsNotification;

/****************************** Global Variables Defination end  ***********************************/


int main(void)
{
	/*Initialization of Keypad and LCD*/
    HAL_LCD_voidInit();
	HAL_Keypad_voidInit();
	/*Create Tasks*/
	xTaskCreate(&LCD_vTask,NULL,LCD_Task_STACK_SIZE,NULL,LCD_Task_PRIORITY,NULL);
	xTaskCreate(&Calc_vTask,NULL,CALC_Task_STACK_SIZE,NULL,CALC_Task_PRIORITY,NULL);
	xTaskCreate(&KeyPad_vTask,NULL,KEYPAD_Task_STACK_SIZE,NULL,KEYPAD_Task_PRIORITY,NULL);
	xTaskCreate(&Timer_vTask,NULL,TIMING_Task_STACK_SIZE,NULL,TIMING_Task_PRIORITY,NULL);
	/*Queue Creation*/
	
	Key2Calc_Queue = xQueueCreate(KEY2CALC_QUEUE_LENGTH, KEY2CALC_QUEUE_WIDTH);
	Key2LCD_Queue  = xQueueCreate(KEY2LCD_QUEUE_LENGTH, KEY2LCD_QUEUE_WIDTH);
	Calc2LCD_Queue = xQueueCreate(CALC2LCD_QUEUE_LENGTH, CALC2LCD_QUEUE_WIDTH);
	Time2LCD_Queue = xQueueCreate(TIM2LCD_QUEUE_LENGTH, TIM2LCD_QUEUE_WIDTH);
	/*Semaphore Creation*/
	
	KeyPad2LCD_ClearNotification        = xSemaphoreCreateBinary();
	KeyPad2Timer_StartCounting          = xSemaphoreCreateBinary();
	Timer2KeyPad_TenSecondsNotification = xSemaphoreCreateBinary();
	KeyPad2Timer_StopCounting			= xSemaphoreCreateBinary();
	/*Start scheduler*/
	
	vTaskStartScheduler();
    for(;;);
}

void KeyPad_vTask(void *Pv)
{
	uint8_t Local_u8ErrorState = pdFALSE;
	uint8_t Local_u8Key = KEYPAD_NO_KEY;
	keypadBuffer_t Local_xKeyPadBuffer;
	BaseType_t Local_xObjectState = pdFALSE;
	while(1)
	{
		Local_u8Key = HAL_Keypad_u8GetKey();
		if(KEYPAD_NO_KEY != Local_u8Key)
		{
			Local_u8ErrorState = Check_vKeypadInput(Local_u8Key,&Local_xKeyPadBuffer);
			if(pdTRUE == Local_u8ErrorState)
			{
				/*Clear LCD Now (wrong Equation)*/
				xSemaphoreGive(KeyPad2LCD_ClearNotification);
			}
			else
			{
				if(1 == Local_xKeyPadBuffer.completeFlag)
				{
					/*Send equation to calc task*/
					xQueueSend(Key2Calc_Queue,&(Local_xKeyPadBuffer.MSG),0);
					Local_xKeyPadBuffer.completeFlag =0;
					/*Stop counting if it's already posted*/
					xSemaphoreGive(KeyPad2Timer_StopCounting);
				}
				else
				{
					/*send to LCD Task*/
					xQueueSend(Key2LCD_Queue,&(Local_xKeyPadBuffer.MSG),0);
					/*Stop counting if it's already posted*/
					xSemaphoreGive(KeyPad2Timer_StopCounting);
				}
			}
		}
		else
		{
			if(1 == Local_xKeyPadBuffer.updateFlag)
			{
				/*means there is equation still in progress
				* Requirement is to monitor the input of incomplete equations, if
					10 sec passed and no activity from user is detected then clear the LCD.
				*/
				xSemaphoreGive(KeyPad2Timer_StartCounting);
			}
		}
		/*Check if LCD clear notification was raised*/
		Local_xObjectState = xSemaphoreTake(Timer2KeyPad_TenSecondsNotification,0);
		if(pdPASS == Local_xObjectState)
		{
			/*Clear LCD*/
			xSemaphoreGive(KeyPad2LCD_ClearNotification);
			Check_vKeypadInput(0,NULL);/*To reset the values pass null to this function*/
		}
		else{/*Nothing*/}
		vTaskDelay(KEYPAD_TASK_PERIODICITY);
	}
}
void Calc_vTask(void *Pv)
{
	BaseType_t Local_xObjectState = pdFALSE;
	equationMSG_t Local_xKeypadMessage;
	equationResultMSG_t Local_xResultMessage;
	while(1)
	{
		Local_xObjectState = xQueueReceive(Key2Calc_Queue,&Local_xKeypadMessage,0);
		if(pdPASS == Local_xObjectState)
		{
			/*Process Data*/
			Local_xResultMessage.equation.firstVal = Local_xKeypadMessage.firstVal;
			Local_xResultMessage.equation.Operator = Local_xKeypadMessage.Operator;
			Local_xResultMessage.equation.secondVal = Local_xKeypadMessage.secondVal;
			switch(Local_xKeypadMessage.Operator)
			{
				case '+':
					/*the - '0' is to convert from char to number*/
					Local_xResultMessage.resultVal = (Local_xKeypadMessage.firstVal -'0') + (Local_xKeypadMessage.secondVal-'0');
					break;
				case '-':
					Local_xResultMessage.resultVal = (Local_xKeypadMessage.firstVal -'0') - (Local_xKeypadMessage.secondVal-'0');
					break;
				case '/':
					if('0' == Local_xKeypadMessage.secondVal)
					{
						/*Can't divide by zero, may handle it in future with semaphore to lcd*/
						Local_xResultMessage.resultVal = 0;
					}
					else
					{
						Local_xResultMessage.resultVal = (Local_xKeypadMessage.firstVal -'0') / (Local_xKeypadMessage.secondVal-'0');	
					}
					
					break;
				case '*':
					Local_xResultMessage.resultVal = (Local_xKeypadMessage.firstVal -'0') * (Local_xKeypadMessage.secondVal-'0');
					break;
				default:
					/*Error*/
					Local_xResultMessage.resultVal = 0;
					break;
			}
			xQueueSend(Calc2LCD_Queue,&Local_xResultMessage,0);
		}
		else
		{
			/*Do Nothing*/
		}
		vTaskDelay(CALC_TASK_PERIODICITY);
	}
}
void LCD_vTask(void *Pv)
{
	BaseType_t Local_xObjectState = pdFALSE;
	uint8_t Local_u8TimeBuffer[TIME_STRING_LENGTH];
	equationResultMSG_t Local_xResultEquation;
	equationMSG_t Local_xCurrentEq;
	
	while(1)
	{
		vTaskDelay(LCD_TASK_PERIODICITY);
		
		Local_xObjectState = xQueueReceive(Time2LCD_Queue,Local_u8TimeBuffer,0);
		if(pdPASS == Local_xObjectState)
		{
			/*Print time*/
			HAL_LCD_voidGoTo(LINE2,0);
			HAL_LCD_voidSendString(Local_u8TimeBuffer);
		}
		else{/*Do Nothing*/}
		/*Check if clear notification was sent*/
		Local_xObjectState  = xSemaphoreTake(KeyPad2LCD_ClearNotification,0);
		if(pdPASS == Local_xObjectState)
		{
			/*Clear LCD LINE 0*/
			HAL_LCD_voidGoTo(LINE1,0);
			HAL_LCD_voidSendString("             ");
		}
		else
		{
			/*Check other two queues result first then keypad*/
			Local_xObjectState = xQueueReceive(Calc2LCD_Queue,&Local_xResultEquation,0);
			if(pdPASS == Local_xObjectState)
			{
				/*Print Result*/
				HAL_LCD_voidGoTo(LINE1,0);
				HAL_LCD_voidSendChar(Local_xResultEquation.equation.firstVal);
				HAL_LCD_voidSendChar(Local_xResultEquation.equation.Operator);
				HAL_LCD_voidSendChar(Local_xResultEquation.equation.secondVal);
				HAL_LCD_voidSendChar('=');
				/*parse result
				* /10 to get first digit as 10 / 10 = 1.
				* %10 to get second digit as 10 %10 = 0.
				*/
				if(10 > Local_xResultEquation.resultVal)
				{
					HAL_LCD_voidSendChar((Local_xResultEquation.resultVal%(uint8_t)10)+'0');	
				}
				else
				{
					HAL_LCD_voidSendChar((Local_xResultEquation.resultVal/(uint8_t)10)+'0');
					HAL_LCD_voidSendChar((Local_xResultEquation.resultVal%(uint8_t)10)+'0');	
				}
				
			}
			else
			{
				Local_xObjectState = xQueueReceive(Key2LCD_Queue,&Local_xCurrentEq,0);
				if(pdPASS == Local_xObjectState)
				{
					/*Print Current input Equation*/					
					HAL_LCD_voidGoTo(LINE1,0);
					HAL_LCD_voidSendChar(Local_xCurrentEq.firstVal);
					if(NO_INPUT_YET != Local_xCurrentEq.Operator)
					{
						HAL_LCD_voidSendChar(Local_xCurrentEq.Operator);
						if(NO_INPUT_YET != Local_xCurrentEq.secondVal)
						{
							HAL_LCD_voidSendChar((Local_xCurrentEq.secondVal));
						}
						else{HAL_LCD_voidSendString("             ");}
					}
					else{HAL_LCD_voidSendString("             ");}
				}
				else
				{
					/*Do Nothing*/
				}
			}
		}
		
		
	}
}
void Timer_vTask(void *Pv)
{
	uint8_t Local_u8Seconds =0;
	uint8_t Local_u8Minutes =0;
	uint8_t Local_u8Houres =0;
	uint8_t Local_u8TimeString[TIME_STRING_LENGTH];
	BaseType_t Local_xObjectState = pdFALSE;
	uint8_t Local_u8Start10Sec = 0;
	static uint8_t TenSec_u8Counter =0;
	while(1)
	{
		vTaskDelay(TIME_TASK_PERIODICITY); /*Periodicity of task is 1Sec*/
		Local_u8Seconds++;
		if(UNIT_TIME_BASE == Local_u8Seconds)
		{
			Local_u8Seconds = 0;
			Local_u8Minutes++;
			if(UNIT_TIME_BASE == Local_u8Minutes)
			{
				Local_u8Minutes =0;
				Local_u8Houres = (Local_u8Houres+1) % 24;
			}
			else{/*Do Nothing*/}
		}
		else{/*Do Nothing*/}
			
		/*Call Time Gather Function*/
		Get_vTimeNow(Local_u8Seconds,Local_u8Minutes,Local_u8Houres,Local_u8TimeString);
		/*Send current time to LCD through (Time2LCD_Queue) */
		Local_xObjectState = xQueueSend(Time2LCD_Queue,Local_u8TimeString,10);
		if(pdFALSE == Local_xObjectState)
		{
			/*Do nothing
			  This may be used in future to perform some actions if sending was failed
			*/
		}
		else{/*Do nothing*/}
		
		/*Check if 10 Sec semaphore was posted*/
		Local_xObjectState = xSemaphoreTake(KeyPad2Timer_StartCounting,0);
		if(pdTRUE == Local_xObjectState)
		{
			/*Start counting 10 sec*/
				Local_u8Start10Sec=1;
		}
		else{/*Do Nothing*/}
		/*Check if stop 10 Sec semaphore was posted*/
		Local_xObjectState = xSemaphoreTake(KeyPad2Timer_StopCounting,0);
		if(pdTRUE == Local_xObjectState)
		{
			/*Stop counting 10 sec*/
			Local_u8Start10Sec=0;
		}
		else{/*Do Nothing*/}
		
		/*Take action if start counting was posted*/
		if(1 == Local_u8Start10Sec)
		{
			TenSec_u8Counter++;
			if(10 == TenSec_u8Counter)
			{
				/*Give indication that 10 sec passed using Timer2KeyPad_TenSecondsNotification */
				xSemaphoreGive(Timer2KeyPad_TenSecondsNotification);
				TenSec_u8Counter=0;
				Local_u8Start10Sec=0;
			}
			else{;}
		}
		else{;}
			
	}
}


/****************************** Private functions implementation  ***********************************/

static void Get_vTimeNow(uint8_t Copy_u8Seconds,uint8_t Copy_u8Min,uint8_t Copy_u8Hours,uint8_t *Copy_pu8String)
{
	uint8_t Local_u8Iterator =0;
	if(NULL == Copy_pu8String)
	{
		/*Null pointer don't process the values*/
	}
	else
	{
		Copy_pu8String[Local_u8Iterator++] = (Copy_u8Hours /(uint8_t)10)+'0';
		Copy_pu8String[Local_u8Iterator++] = (Copy_u8Hours %(uint8_t)10)+'0';
		Copy_pu8String[Local_u8Iterator++] = ':';
		Copy_pu8String[Local_u8Iterator++] = (Copy_u8Min /(uint8_t)10)+'0';
		Copy_pu8String[Local_u8Iterator++] = (Copy_u8Min %(uint8_t)10)+'0';
		Copy_pu8String[Local_u8Iterator++] = ':';
		Copy_pu8String[Local_u8Iterator++] = (Copy_u8Seconds /(uint8_t)10)+'0';
		Copy_pu8String[Local_u8Iterator++] = (Copy_u8Seconds %(uint8_t)10)+'0';
		Copy_pu8String[Local_u8Iterator] = '\0'; /*terminate string with null*/
	}
}

static uint8_t Check_vKeypadInput(uint8_t Copy_u8KeyValue,keypadBuffer_t *Copy_pxKeypadBuffer)
{
	uint8_t Local_u8ErrorState = pdFALSE;
	static uint8_t Key_u8PositionIndicator=0;
	
	/*This flag to help monitor 10 sec without any activity on calculator and if so the LCD will be
		cleared.
		the flag will only be cleared if user entered '='.
	*/
	Copy_pxKeypadBuffer->updateFlag = 1;
	if(KEYPAD_SMALLEST_VALUE <= Copy_u8KeyValue && KEYPAD_LARGEST_VALUE >= Copy_u8KeyValue)
	{
		/*Number was pressed*/
		if(0 == Key_u8PositionIndicator)
		{
			/*First number*/
			Copy_pxKeypadBuffer->MSG.firstVal = Copy_u8KeyValue;
			/*Indicator that only first number was entered (this will help in LCD task)*/
			Copy_pxKeypadBuffer->MSG.Operator = NO_INPUT_YET;
			Copy_pxKeypadBuffer->MSG.secondVal = NO_INPUT_YET;
		}
		else if(1 == Key_u8PositionIndicator)
		{
			/*Won't accept it*/
			Local_u8ErrorState = pdTRUE;
		}
		else if(2 == Key_u8PositionIndicator)
		{
			Copy_pxKeypadBuffer->MSG.secondVal = Copy_u8KeyValue;
		}
		else
		{
			Local_u8ErrorState = pdTRUE;
		}
	}
	else if(KEYPAD_CALCULATE_VALUE == Copy_u8KeyValue)
	{
		/*Start calculation*/
		if(3 == Key_u8PositionIndicator)
		{
			Copy_pxKeypadBuffer->completeFlag = 1;	
			
		}
		else
		{
			Local_u8ErrorState = pdTRUE;
		}
		
		
	}
	else
	{
		/*Operator*/
		if(1 == Key_u8PositionIndicator)
		{
			Copy_pxKeypadBuffer->MSG.Operator = Copy_u8KeyValue;
			/*Indicator that only first number and operator were entered (this will help in LCD task)*/
			Copy_pxKeypadBuffer->MSG.secondVal = NO_INPUT_YET;
		}
		else
		{
			/*Report Error wrong equation*/
			Local_u8ErrorState = pdTRUE;
		}
	}
	/*Increase indicator to move to next operand or operator when calling this function again*/
	if(pdTRUE == Local_u8ErrorState)
	{
		Key_u8PositionIndicator =0;	
	}
	else
	{
		Key_u8PositionIndicator = (Key_u8PositionIndicator +1 )%4;	
	}
	
	
	return (Local_u8ErrorState);
}