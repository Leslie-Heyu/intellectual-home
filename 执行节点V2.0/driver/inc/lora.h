#include "stm32f10x.h"
#include "usart.h"

#define RX_BUF_MAX_LEN     1024                                     //�����ջ����ֽ���

#pragma anon_unions
extern struct  STRUCT_USARTx_Fram                                  //��������֡�Ĵ���ṹ��
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
  union {                                   //�����union�ĳ���struct
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
 }; 
	
} strEsp8266_Fram_Record;


extern u8 USART3_TX_BUF[1024]; 	//���ͻ���,���1024�ֽ�,Ҫ���͸�wifiģ���

extern struct STRUCT_USARTx_Fram Lora_Record;

void lora_init();
void USART3_Config(unsigned bound);   																			//��ʼ��lora����
void USART3_NVIC_Configuration();																									//����lora����NVIC�ж�
void Lora_Printf(char *send);                                                   //���������lora
void handle_lora();
char* lora_get();
void lora_ctr(char num);