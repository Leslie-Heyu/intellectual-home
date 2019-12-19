/****************************************Copyright (c)****************************************************
** ִ�нڵ�usart3��lora��-->��۽ڵ�usart2��lora��                                     
** ��۽ڵ�usart3��wifi������·����                                
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			
** Last modified Date:          
** Last Version:		   
** Descriptions:							
**--------------------------------------------------------------------------------------------------------
** Created by:			FiYu
** Created date:		2015-12-15
** Version:			    1.0
** Descriptions:		ESP8266 WiFi����ͨ��ʵ��				
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:				
** Descriptions:		
** Rechecked by:			
**********************************************************************************************************/
 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "test.h"
#include "lora.h"
 
 /**************************************************************************************
 * ��  �� : GPIO_LED��ʼ��
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 						 
		
  GPIO_DeInit(GPIOB);	 //������GPIOB�Ĵ�������Ϊȱʡֵ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB , GPIO_Pin_9);   //��ʼ״̬��Ϩ��ָʾ��
}

/**************************************************************************************
 * ��  �� : MAIN����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
int main ( void )
{
	LED_Configuration ();
  USARTx_Config ();                                        //��ʼ������1
	USART1_Init();
	SysTick_Init ();                                         //���� SysTick Ϊ 1ms �ж�һ�� 
	ESP8266_Init ();                                         //��ʼ��WiFiģ��ʹ�õĽӿں�����
	lora_init();																						//��ʼ��lora���� 
  printf("sink node\r\n");
  ESP8266_StaTcpClient_UnvarnishTest ();
  while (1)
	{
		;
	}
	
	
}


/*********************************************END OF FILE**********************/
