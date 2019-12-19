
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "usart.h"

//#define PRINTF(...)  printf()do{}while(0)
/*����ȫ�ֱ���*/
uint16_t TIM5CH1_CAPTURE_VAL;//TIM5_IRQHeader
uint8_t TIM5CH1_CAPTURE_STA;//TIM5_IRQHearder
//�������
float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
extern float  Acc_angle,Gry_vivi;
extern float Angle,Gyro_x;         //С���˲�����б�Ƕ�/���ٶ�	
extern float Angle_x_temp;  //�ɼ��ٶȼ����x��б�Ƕ�
extern float Angle_y_temp;  //�ɼ��ٶȼ����y��б�Ƕ�
extern float Angle_z_temp;
extern float Angle_X_Final; //X������б�Ƕ�
extern float Angle_Y_Final; //Y������б�Ƕ�
extern float Angle_Z_Final; //Z������б�Ƕ�
extern float Gyro_x;		 //X�������������ݴ�
extern float Gyro_y;        //Y�������������ݴ�
extern float Gyro_z;		 //Z�������������ݴ�
extern float A_P,A_R,A2_P;

int main(void)
{
	unsigned long a;
	int x;
	SystemInit();
	LED_Init();
	delay_init();
	Usart_Init(115200);
	while(1)
	{
		a=ReadCount();
		printf("%d\n",a);
		delay_ms(1000);
	}
	return 0;
}