#include "bluetooth.h"
#include "led.h"
#include "makefile.h"
#include "buzzer.h"

u8 USART3_RX_BUF[USART3_REC_LEN]; //���������洢���յ������ݣ���USART3_REC_LENΪ����ܽ��յ��ֽ��޶�
__align(8) u8 USART3_TX_BUF[USART3_REC_LEN]; 	//���ͻ���,���USART3_REC_LEN�ֽ�
u16 USART3_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ

int count3=0;

//������ʼ��
void bluetooth_init()
{
	NVIC_InitTypeDef    NVIC_InitStructure; 					   //�������������жϵĽṹ�����

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                //�ж����ȼ�����  ��ռʽ���ȼ�������Ϊ2λ����Ӧ���ȼ�ռ2λ
	
  //����3�ж����ȼ�����(��������)
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			   //ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	   //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;             //ָ����Ӧ���ȼ���2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //���ж�
	NVIC_Init(&NVIC_InitStructure);							       //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	
	USART3_Config(9600);//�������մ���3��ʼ��
}

void USART3_IRQHandler()//����3���ж�
{ 
	 u8 Res;//��ʱ������յ�����
	  int i=0;
	   if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����3�����ж�
	  {
	     Res=USART_ReceiveData(USART3);//���յ����ݷŽ�res		
			if(Res==0x0d)
			{
			  printf("�����յ���Ϣ:%s\r\n",USART3_RX_BUF);
				make(USART3_RX_BUF);//ִ����Ӧ��makefile
				blcontrol(USART3_RX_BUF[0]-48);
//				usart3_SendByte('o');
//				usart3_SendByte('k');
			USART3_RX_STA=count3;
				for(i=0;i<USART3_RX_STA;i++)
				{
					USART3_RX_BUF[i]=0x00;
				}
			count3=0;
			}
			else
			{
				if(Res==0x0a)
				{}
				else
				{
					USART3_RX_BUF[count3]=Res;
					count3++;
				}
			}
		 USART_ClearITPendingBit(USART3,USART_IT_RXNE);//��������жϱ�־
			}
}

//����ֵ���������յ����ַ���/����
char* blrec()
{
		return (*(& USART3_RX_BUF));
}

//����:����Ҫ���͵��ַ���/����
void blsend(u8 *send)
{
	while((*send)!=0x00)
	{
		usart3_SendByte(*send);
		send++;
	}
}

//���ܣ�ͨ����ͬ������������Ӧ����
//����:����Ĳ���

//1 ���� 2 �ص� 3 ���������� 4ֹͣ���� 5 ���ٶȴ�������ʼ���� 6 ֹͣ���� 7����ʱ�� 8������ʪ�� 9 ���͵�ǰѹ��ֵ
int blcontrol(char num)//1�ɹ�����
{
	switch(num)
	{
		case 1:LED_ON();blsend("1");break;
		case 2:LED_OFF();blsend("2");break;
		case 3:BEEP_ON();blsend("3");break;
		case 4:BEEP_OFF();;blsend("4");break;
		case 5:mpu6050_open();;blsend("5");break;
		case 6:mpu6050_close();blsend("6");break;
		case 7:blsend("7");sendtime();break;
		case 8:blsend("8");sendsht();break;
		case 9:blsend("9");sendpress();break;
		case 0:blsend("0");break;
		default:break;
	}
}

void sendtime()
{
	char dis[60];
	sprintf(dis,"current time:%d.%d.%d  %d:%d:%d\r\n",get_time().year,get_time().month,get_time().day,get_time().hour,get_time().minute,get_time().second);
	blsend(dis);
}

void testblue(void)
{
	int i=0;
	if(USART3_RX_STA)
	{
		if(USART3_RX_BUF[0]=='1'&&USART3_RX_BUF[1]=='2')
		{
			LED_ON();
		}
		if(USART3_RX_BUF[0]=='0'&&USART3_RX_BUF[1]=='1')
		{
			LED_OFF();
		}
		USART3_RX_STA=0;
	}
}