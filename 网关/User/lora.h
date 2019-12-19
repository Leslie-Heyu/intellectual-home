#include "stm32f10x.h"
#include "common.h"
#include "bsp_esp8266.h"

extern u8 USART2_TX_BUF[1024]; 	//���ͻ���,���1024�ֽ�,Ҫ���͸�wifiģ���

extern struct STRUCT_USARTx_Fram Lora_Record;

void lora_init();
void USART2_Config(unsigned bound);   																			//��ʼ��lora����
void USART2_NVIC_Configuration();																									//����lora����NVIC�ж�
void Lora_Printf(char *send);                                                   //���������lora