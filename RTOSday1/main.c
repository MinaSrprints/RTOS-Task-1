#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "LCD.h"
#include "lcd_4bit.h"
#include "keypad.h"
TaskHandle_t init_handel = NULL;
TaskHandle_t task1_handel = NULL;
TaskHandle_t task2_handel = NULL;
TaskHandle_t task3_handel = NULL;
uint8 key='a';
enum
{
	WELCOME_STATE,
	PW_STATE,
	CHECK_STATE,
	END_STATE
}task1state;
uint8 Task1_state=WELCOME_STATE;
void InitTask(void);
void Task2(void);
void Task1(void);
void Task3(void);
uint8 PressFlag=0;
uint8 count = 0;
uint8 Saved_PW[6]={1,2,3,4,5,6};
uint8 Entered_PW[6];
int main(void)
{
	xTaskCreate((TaskFunction_t)InitTask,"init",100,NULL,4,&init_handel);
	xTaskCreate((TaskFunction_t)Task2,"task1",40,NULL,3,&task2_handel);
	xTaskCreate((TaskFunction_t)Task1,"task2",250,NULL,3,&task1_handel);
	xTaskCreate((TaskFunction_t)Task3,"task3",80,NULL,3,&task3_handel);


	/* Start Scheduler */
	vTaskStartScheduler();

	while(1)
	{

	}
	return 0;
}
void InitTask(void)
{
	for(;;)
	{
		LED_Init();
		vTaskDelay(10);
		lcd_init();
		vTaskDelay(10);
		KeyPad_init();
		vTaskDelay(10);
		vTaskSuspend(init_handel);

	}
}
void Task2(void)
{
	for(;;)
	{
		LED_Toggle();
		vTaskDelay(999);
	}
}
void Task1(void)
{
static uint8 count=0;
static uint8 check_counter=0;
static uint8 check_correct_digit=0;
static uint8 PW_correct=0;
	for(;;)
	{
		vTaskDelay(200);
		switch (Task1_state)
		{
		case WELCOME_STATE:
			lcd_clrScreen();
			lcd_disp_string_xy("enter pw",0,0);
			Task1_state=PW_STATE;
			break;
		case PW_STATE:
			if(PressFlag==1)
			{
				if(key==7||key==9)
				{
					Task1_state=WELCOME_STATE;
				}
				else
				{
					Entered_PW[count]=key;
					lcd_disp_string_xy("*",1,count);
					count++;
					if(count==6)
					{
						count=0;
						Task1_state=CHECK_STATE;
					}
					else
					{
						Task1_state=PW_STATE;
					}
				}
			}
			break;
		case CHECK_STATE:
			for(check_counter=0;check_counter<=5;check_counter++)
			{
				if(Entered_PW[check_counter]==Saved_PW[check_counter])
				{
					check_correct_digit++;
				}
			}
			if(check_correct_digit==5)
			{
				PW_correct=1;
				Task1_state=END_STATE;
			}
			else
			{
				PW_correct=0;
				Task1_state=END_STATE;
			}
			break;
		case END_STATE:
			if(PW_correct==1)
			{
				lcd_clrScreen();
				lcd_disp_string_xy("correct pw :D",0,0);
				lcd_disp_string_xy("welcome :D",1,0);
				vTaskDelay(3000);
				Task1_state=WELCOME_STATE;
			}
			else
			{
				lcd_clrScreen();
				lcd_disp_string_xy("incorrect pw :X",0,0);
				lcd_disp_string_xy("renter ur pw :X",1,0);
				vTaskDelay(3000);
				Task1_state=WELCOME_STATE;
			}
			break;
		}
		//
		//		if(PressFlag==0)
		//		{
		//
		//		}
		//		else
		//		{
		//			lcd_clrScreen();
		//			lcd_displayChar(key+'0');
		//		}
		//		vTaskDelay(100);

	}
}
void Task3(void)
{

	for(;;)
	{
		key=KeyPad_getPressedKey();
		if(key=='a')
		{
			PressFlag=0;
		}
		else
		{
			PressFlag=1;
		}
		vTaskDelay(200);
	}
}
