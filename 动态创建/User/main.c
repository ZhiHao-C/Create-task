//#include "stm32f10x.h"                  // Device header
#include "string.h"
#include <stdio.h>

#include "bps_led.h"
#include "bps_usart.h"

#include "FreeRTOS.h"
#include "task.h"

/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED_Task1������ */
static TaskHandle_t LED_Task1_Handle = NULL;
/* LED_Task2������ */
static TaskHandle_t LED_Task2_Handle = NULL;






//��������
static void LED_Task1(void* parameter);
static void LED_Task2(void* parameter);
static void AppTaskCreate(void);

static void BSP_Init(void)
{
	/* 
	* STM32 �ж����ȼ�����Ϊ 4���� 4bit ��������ʾ��ռ���ȼ�����ΧΪ��0~15 
	* ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ� 
	* ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ� 
	*/ 
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 ); 
	LED_GPIO_Config();
	USART_Config();
	
	//����
//	led_G(on);
//	printf("���ڲ���");
}

int main()
{
	BaseType_t xReturn = NULL;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	BSP_Init();
	printf("����ȫϵ�п�����-FreeRTOS-��̬��������!\r\n");
	printf("ʵ�ֵ�����Ϊ�����̵Ƶ���˸\r\n");
	
	  /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
																							
	if(xReturn==pdPASS)
	{
		printf("��ʼ���񴴽��ɹ�\r\n");
		vTaskStartScheduler();
	}
	else 
	{
		return -1;
	}
	while(1)
	{
		
	}

}


//���������1
static void LED_Task1(void* parameter)
{
	while(1)
	{
		led_G(1);
		vTaskDelay(500);         //��ʱ500��ticks,����һ��tickΪ1ms
		led_G(0);
		vTaskDelay(500);         //��ʱ500��ticks,����һ��tickΪ1ms
	}    
}

//���������2
static void LED_Task2(void* parameter)
{
	while(1)
	{
		led_R(1);
		vTaskDelay(1000);         //��ʱ500��ticks,����һ��tickΪ1ms
		led_R(0);
		vTaskDelay(1000);         //��ʱ500��ticks,����һ��tickΪ1ms
	}    
}



static void AppTaskCreate(void)
{
	BaseType_t xReturn = NULL;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	taskENTER_CRITICAL();           //�����ٽ���
	
	xReturn=xTaskCreate((TaskFunction_t	)LED_Task1,		//������
															(const char* 	)"LED_Task1",		//��������
															(uint16_t 		)512,	//�����ջ��С
															(void* 		  	)NULL,				//���ݸ��������Ĳ���
															(UBaseType_t 	)2, 	//�������ȼ�
															(TaskHandle_t*  )&LED_Task1_Handle);/* ������ƿ�ָ�� */ 
															
															
	if(xReturn == pdPASS)/* �����ɹ� */
		printf("LED_Task1���񴴽��ɹ�!\n");
	else
		printf("LED_Task1���񴴽�ʧ��!\n");
	
		xReturn=xTaskCreate((TaskFunction_t	)LED_Task2,		//������
															(const char* 	)"LED_Task2",		//��������
															(uint16_t 		)512,	//�����ջ��С
															(void* 		  	)NULL,				//���ݸ��������Ĳ���
															(UBaseType_t 	)3, 	//�������ȼ�
															(TaskHandle_t*  )&LED_Task2_Handle);/* ������ƿ�ָ�� */ 
															
	if(xReturn == pdPASS)/* �����ɹ� */
		printf("LED_Task2���񴴽��ɹ�!\n");
	else
		printf("LED_Task2���񴴽�ʧ��!\n");
	
	vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
	
	taskEXIT_CRITICAL();            //�˳��ٽ���
}


//��̬�����������Ҫ
///**
//  **********************************************************************
//  * @brief  ��ȡ��������������ջ��������ƿ��ڴ�
//	*					ppxTimerTaskTCBBuffer	:		������ƿ��ڴ�
//	*					ppxTimerTaskStackBuffer	:	�����ջ�ڴ�
//	*					pulTimerTaskStackSize	:		�����ջ��С
//  * @author  fire
//  * @version V1.0
//  * @date    2018-xx-xx
//  **********************************************************************
//  */ 
//void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
//								   StackType_t **ppxIdleTaskStackBuffer, 
//								   uint32_t *pulIdleTaskStackSize)
//{
//	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* ������ƿ��ڴ� */
//	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* �����ջ�ڴ� */
//	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* �����ջ��С */
//}



///**
//  *********************************************************************
//  * @brief  ��ȡ��ʱ������������ջ��������ƿ��ڴ�
//	*					ppxTimerTaskTCBBuffer	:		������ƿ��ڴ�
//	*					ppxTimerTaskStackBuffer	:	�����ջ�ڴ�
//	*					pulTimerTaskStackSize	:		�����ջ��С
//  * @author  fire
//  * @version V1.0
//  * @date    2018-xx-xx
//  **********************************************************************
//  */ 
//void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
//									StackType_t **ppxTimerTaskStackBuffer, 
//									uint32_t *pulTimerTaskStackSize)
//{
//	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* ������ƿ��ڴ� */
//	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* �����ջ�ڴ� */
//	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* �����ջ��С */
//}
