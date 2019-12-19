#include "led.h"
/**Ӳ�����ӣ�PC10->D3 PC11->D4 PC12->D5 PD2->D6 PA8->D1
  ************************************
  * @brief  Led��IO�ڵĳ�ʼ��
  * @param  None
  * @retval None
	************************************
*/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 
	
	GPIO_DeInit(GPIOB);	 //������GPIOB�Ĵ�������Ϊȱʡֵ		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_9);    //��PB9�����ø�,��ʼ״̬ΪϨ��ָʾ��
}

/*************************
��led��
***************************/
void LED_ON(void)
{
	GPIO_ResetBits(GPIOB , GPIO_Pin_9);   //��PB9�����õ�,����ָʾ��
}
/*************************************
�ر�led��
*********************************/
void LED_OFF(void)
{

	GPIO_SetBits(GPIOB , GPIO_Pin_9);    //��PB9�����ø�,Ϩ��ָʾ��
}
