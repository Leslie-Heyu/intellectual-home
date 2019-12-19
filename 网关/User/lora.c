#include "lora.h"
/*��۽ڵ��lora�õ���usart2,����usart2*/

u8 USART2_RX_BUF[1024]; //���������洢���յ������ݣ�����ܽ���1024�ֽ�
u16 USART2_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
int count2=0;
#define RX_BUF_MAX_LEN        1024
struct  STRUCT_USARTx_Fram Lora_Record = { 0 };//   lora���յ�������

u8 USART2_TX_BUF[1024];
void lora_init()
{
	USART2_Config(9600);
	USART2_NVIC_Configuration();
}

//����2��ʼ��(lora����)9600 8N1
void USART2_Config(unsigned int bound)   //��ʼ��lora����
{
	GPIO_InitTypeDef    GPIO_InitStructure;	   //���ڶ˿����ýṹ�����
	USART_InitTypeDef   USART_InitStructure;   //���ڲ������ýṹ�����

	//ʹ�� USART2 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//�򿪴��ڸ���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);   //��PA�˿�ʱ��

	//��USART2 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //ѡ���ĸ�IO�� ��ѡ��PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //��ʼ��GPIOA

	//��USART2 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //ѡ���ĸ�IO�� ��ѡ��PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //��ʼ��GPIOA
	 
	//����USART2����
	USART_InitStructure.USART_BaudRate = bound;	                    //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART2, &USART_InitStructure);                       //��ʼ��USART2
	
	//�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART2�����ж�
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART2�����ж�
	USART_ITConfig ( USART2, USART_IT_IDLE, ENABLE ); //ʹ�ܴ������߿����ж� 	

	//ʹ�� USART2�� �������
	USART_Cmd(USART2, ENABLE);                             // USART2ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    USART_ClearFlag(USART2, USART_FLAG_TC);                //�崮��2���ͱ�־
}

//����lora����NVIC�ж�
void USART2_NVIC_Configuration()//����lora����NVIC�ж�
{
	NVIC_InitTypeDef    NVIC_InitStructure; 					   //�������������жϵĽṹ�����

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                //�ж����ȼ�����  ��ռʽ���ȼ�������Ϊ2λ����Ӧ���ȼ�ռ2λ
	
  //����2�ж����ȼ�����(lora����)
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			   //ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	   //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;             //ָ����Ӧ���ȼ���2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //���ж�
	NVIC_Init(&NVIC_InitStructure);							       //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

}

//����:Ҫͨ��lora����ȥ������
//ĩβ��0x0d��0x0a????
void Lora_Printf(char *send)
{
	while((*send)!=0x00)
	{
		usart2_SendByte(*send);
		send++;
	}
//	usart2_SendByte(0x0d);
	//usart2_SendByte(0x0a);
}


void USART2_IRQHandler()//����2(lora)���ж�,���ж�һ������һ��lora����
{ 
	  u8 Res;//��ʱ������յ�����
	  int i=0;
	   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����2�����ж�
	  {
	     Res=USART_ReceiveData(USART2);//���յ����ݷŽ�res	
		if ( Lora_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //Ԥ��1���ֽ�д������
		{
			Lora_Record .Data_RX_BUF [ Lora_Record .InfBit .FramLength ++ ]  = Res;//���ݱ�д����Data_RX_BUF
		}

	}
	 	 
	if ( USART_GetITStatus( USART2, USART_IT_IDLE ) == SET )                                         //����֡�������
	{
    Lora_Record .InfBit .FramFinishFlag = 1;
		
		Res= USART_ReceiveData( USART2 );                                                              //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)	
		 
  }	
}

//void USART2_IRQHandler()//����2(lora)���ж�,���ж�һ������һ��lora����
//{ 
//	  u8 Res;//��ʱ������յ�����
//	  int i=0;
//	   u8 *s = "+IPD,10,192.168.191.1,5000:test wifi is whether ok? ";
//	   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����2�����ж�
//	  {
//	     Res=USART_ReceiveData(USART2);//���յ����ݷŽ�res	
//			if(Res==0x0d)
//			{
//		//	  printf("loraģ���յ���Ϣ:%s\r\n",USART2_RX_BUF);
//				//������ϣ���wifiģ��ת��
//				for(i=0;i<count2;i++)
//				{
//					USART2_TX_BUF[i]=USART2_RX_BUF[i];
//				}
//				  sendtowifi(s);
//			//	sendtowifi(USART2_TX_BUF);
//				//+IPD,10,192.168.191.1,5000:test wifi is whether ok? 
//				//wifiҪ�����ݸ�ʽ"+IPD,<len>[,<remote IP>,<remoteport>]:<data>"
//	//			USART_printf(USART3,USART2_TX_BUF);
//				//�������ݷ��ͺ󣬾�Ҫ�������
//			  USART2_RX_STA=count2;
//				for(i=0;i<USART2_RX_STA;i++)
//				{
//					USART2_RX_BUF[i]=0x00;
//				}
//			  count2=0;
//			 }
//			else
//			{
//				if(Res==0x0a)
//				{}
//				else
//				{
//					USART2_RX_BUF[count2]=Res;
//					count2++;
//				}
//			}
//		 USART_ClearITPendingBit(USART2,USART_IT_RXNE);//��������жϱ�־
//		}
//}

