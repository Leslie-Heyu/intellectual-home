#include "lora.h"
/*ִ�нڵ��lora�õ���usart3,����usart3*/

u8 USART3_RX_BUF[1024]; //���������洢���յ������ݣ�����ܽ���1024�ֽ�
u16 USART3_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
int count3=0;
#define RX_BUF_MAX_LEN        1024
struct  STRUCT_USARTx_Fram Lora_Record = { 0 };//   lora���յ�������

u8 USART3_TX_BUF[1024];
void lora_init()
{
	USART3_Config(9600);
	USART3_NVIC_Configuration();
}

//����3��ʼ��(lora����)9600 8N1
void USART3_Config(unsigned int bound)   //��ʼ��lora����
{
		GPIO_InitTypeDef GPIO_InitStructure;	  //���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;	  //���ڲ������ýṹ�����

	//ʹ�� USART3 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//�򿪴��ڸ���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //��PB�˿�ʱ��
  	
	//��USART3 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //ѡ���ĸ�IO�� ��ѡ��PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //��ʼ��GPIOB
	
	//��USART3 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //ѡ���ĸ�IO�� ��ѡ��PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //��ʼ��GPIOB
	  
	//����USART3����
//		USART_InitStructure.USART_BaudRate = 115200;	                    //���������ã�115200
	USART_InitStructure.USART_BaudRate =bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);                       //��ʼ��USART3
   
	//�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART3�����ж�
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART3�����ж�
	USART_ITConfig ( USART3, USART_IT_IDLE, ENABLE ); //ʹ�ܴ������߿����ж� 	

	//ʹ�� USART3�� �������
	USART_Cmd(USART3, ENABLE);                             // USART3ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    USART_ClearFlag(USART3, USART_FLAG_TC);                //�崮��3���ͱ�־
}

//����lora����NVIC�ж�
void USART3_NVIC_Configuration()//����lora����NVIC�ж�
{
	NVIC_InitTypeDef    NVIC_InitStructure; 					   //�������������жϵĽṹ�����

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                //�ж����ȼ�����  ��ռʽ���ȼ�������Ϊ2λ����Ӧ���ȼ�ռ2λ
	
  //����3�ж����ȼ�����(lora����)
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			   //ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;             //ָ����Ӧ���ȼ���2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //���ж�
	NVIC_Init(&NVIC_InitStructure);							       //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

}

//����:Ҫͨ��lora����ȥ������
//ĩβ��0x0d��0x0a
void Lora_Printf(char *send)
{
	while((*send)!=0x00)
	{
		usart3_SendByte(*send);
		send++;
	}
	usart3_SendByte(0x0d);
	usart3_SendByte(0x0a);
}

//����lora���յ����ַ���
//�ɹ��򷵻��ַ����׵�ַ��ʧ���򷵻�NULL
char* lora_get()
{
		if(Lora_Record .InfBit.FramFinishFlag)	
		{
			int i=0;
	//		USART_ITConfig (USART3, USART_IT_RXNE, DISABLE ); //ʧ�ܴ��ڽ����ж�
			Lora_Record .Data_RX_BUF [ Lora_Record .InfBit .FramLength ]  = '\0';//֡β����0x00,Ϊ�˶Ͽ��ַ���
			
			Lora_Record .InfBit .FramLength = 0;//֡����֡������־����0���´δ�buf��0����ʼд
	    Lora_Record .InfBit .FramFinishFlag = 0;	
			
			  	return Lora_Record .Data_RX_BUF;
	//		USART_ITConfig (USART3, USART_IT_RXNE, ENABLE ); //�ܴ��ڽ����ж�  ������������ϵͳ�ͱ���
		}
		else return NULL;
}

void USART3_IRQHandler()//����3(lora)���ж��¼�,���ж��¼�����һ��lora����,�������ݲ���Ӧ
{ 
	  u8 Res;//��ʱ������յ�����
	  int i=0;
	   if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����3�����ж�
	  {
					Res=USART_ReceiveData(USART3);//���յ����ݷŽ�res	
					if ( Lora_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //Ԥ��1���ֽ�д������
					{
							Lora_Record .Data_RX_BUF [ Lora_Record .InfBit .FramLength  ]  = Res;//���ݱ�д����Data_RX_BUF
							Lora_Record .InfBit .FramLength++;
					}	
	  }
		if ( USART_GetITStatus( USART3, USART_IT_IDLE ) == SET )                                         //����֡�������
		{
			Lora_Record .InfBit .FramFinishFlag = 1;
		 	printf("���յ��ַ���%s\r\n%s\r\n",Lora_Record .Data_RX_BUF,Lora_Record .Data_RX_BUF+1);																																									//��Ӧ�¼��������ظ�
						
			Res= USART_ReceiveData( USART3 );                                                              //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)	
			handle_lora();
		}
	
}
//����lora���յ��Ŀ�����/�ַ���
//��һλ�����֣�ʣ��λ������
void handle_lora()
{
	   char * s="";

			s=lora_get();	//�õ�������
			//��������
			if(s!=NULL)
			{
				 printf("lora get message:%s\r\n",s);
				//���ݿ����ִ�������¼�
				 lora_ctr(s[0]);
			//	Lora_Printf(s);
				//��������
				printf("I will handle data about %s\r\n",s+1);//�˴���ʱ��ӡ����
			}
			
			else
			{
				printf("not get message!\r\n");//�����쳣��û�õ�������
			}
}

//���ܣ�ͨ����ͬ������������Ӧ����
//����:����Ĳ���

//1 ���� 2 �ص� 3 ���������� 4ֹͣ���� 5 ���ٶȴ�������ʼ���� 6 ֹͣ���� 7����ʱ�� 8������ʪ�� 9 ���͵�ǰѹ��ֵ
void lora_ctr(char num)
{
	switch(num)
	{
		case '1':Lora_Printf("1");LED_ON();break;
		case '2':Lora_Printf("2");LED_OFF();break;
		case '3':Lora_Printf("3");BEEP_OFF();BEEP_ON();break;
		case '4':Lora_Printf("4");BEEP_OFF();break;
		case '5':Lora_Printf("5");mpu6050_open();break;
		case '6':Lora_Printf("6");mpu6050_close();break;
		case '7':Lora_Printf("7");sendtime();break;
		case '8':Lora_Printf("8");sendsht();break;
		case '9':Lora_Printf("9");sendpress();break;
		case '0':Lora_Printf("0");break;
		default:break;
	}
}