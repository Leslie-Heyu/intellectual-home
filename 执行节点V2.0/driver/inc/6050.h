
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "usart.h"

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
//#include "uart_1.h"
//#include "dma.h"
//#include "RS485.h"
//#include "74hc595.h"
#include "string.h"
#include "rtc_time.h"

/**************************
ģ��������Ϣ��
ѡ��оƬ�� stm32f103vc
������Ϣ��stm32f10x_hd.s
�̼��⣺stm32�̼���v3.5
***************************/

#define led_PB12   PBout(12)	   //λ�󶨣�λ����PB12����ΪLED����������ٶ����
extern float a[3],w[3],angle[3],T;
extern unsigned char Re_buf[11],temp_buf[11],counter;
extern unsigned char sign;
void mpu6050_init();
void mpu6050_close();
void mpu6050_open();
int getdata(void);
int monitor(float *ax,float *a);//����㷨
void mymemcpy(unsigned char * Re_buf,unsigned char * Temp,int count);