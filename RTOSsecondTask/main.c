#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "LCD.h"
#include "lcd_4bit.h"
#include "keypad.h"
#include "button.h"
#include "semphr.h"
TaskHandle_t init_handel = NULL;
TaskHandle_t task1_handel = NULL;
TaskHandle_t task2_handel = NULL;
TaskHandle_t task3_handel = NULL;
SemaphoreHandle_t xSemaphore;
SemaphoreHandle_t xSemaphore1;

void InitTask(void);
void Task2(void);
void Task1(void);
void Task3(void);

int main(void)
{
	xTaskCreate((TaskFunction_t)InitTask,"init",300,NULL,4,&init_handel);
	xTaskCreate((TaskFunction_t)Task2,"task1",300,NULL,5,&task2_handel);
	xTaskCreate((TaskFunction_t)Task1,"task2",200,NULL,3,&task1_handel);
	xTaskCreate((TaskFunction_t)Task3,"task3",200,NULL,4,&task3_handel);

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
		BUTTON_Init();
		vTaskDelay(10);
		lcd_init();
		vTaskDelay(10);
		/* Attempt to create a semaphore. */
		xSemaphore = xSemaphoreCreateBinary();
		vTaskDelay(10);
		xSemaphore1 = xSemaphoreCreateBinary();
		vTaskDelay(10);
		vTaskSuspend(init_handel);
	}
}
void Task1(void)
{
	vTaskDelay(40);
	static uint8 lcd_flag =0;
	for(;;)
	{
		if(xSemaphoreTake(xSemaphore,0) == pdTRUE)
		{
			lcd_flag=1;
			if(lcd_flag==1)
			{
				xSemaphoreGive( xSemaphore1 );
			}
			else
			{
				lcd_clrScreen();
			}
		}
		else
		{
			lcd_clrScreen();
		}
		vTaskDelay(100);
	}
}
void Task2(void)
{
	vTaskDelay(40);
	for(;;)
	{
		if(!BUTTON_GitButtonState())
		{
			vTaskDelay(10);
			if(!BUTTON_GitButtonState())
			{
				xSemaphoreGive( xSemaphore );
				//				vTaskDelay(1);
			}
		}
		vTaskDelay(50);
	}
}
void Task3(void)
{
	vTaskDelay(40);
	for(;;)
	{
		if (xSemaphoreTake( xSemaphore1 ,0) == pdTRUE)
		{
			lcd_disp_string_xy("overriten",1,0);
			vTaskDelay(100);

		}
		else
		{
			lcd_disp_string_xy("Hellp LCD",0,0);
			vTaskDelay(100);

		}
		vTaskDelay(200);
	}

}
