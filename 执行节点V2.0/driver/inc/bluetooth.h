#include "stm32f10x.h"
#include "6050.h"

void bluetooth_init();
char* blrec();
void blsend(u8 *send);
int blcontrol(char num);
//void sendtime();
void testblue(void);

#define USART3_REC_LEN  200  	//�����������ֽ��� 200
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�
extern u16 USART3_RX_STA;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ