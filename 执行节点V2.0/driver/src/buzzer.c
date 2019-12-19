//PB12>beep
#include "buzzer.h"
 /************************************
  * @brief  ��������IO�ڵĳ�ʼ��
  * @param  None
  * @retval None
	*************************************/
void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;//����ṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//�ⲿʱ��ʹ��

	//PB12 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ��ͨ�������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure1);//��ʼ��
}

void BEEP_ON()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);//��1
}

void BEEP_OFF()
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//��0
}