#include "6050.h"
/*************************************************
����: int getdata(void)
����: ��ȡ����������
����: ��
����: ��
**************************************************/
float a[3],w[3],angle[3],T;
unsigned char Re_buf[11],temp_buf[11],counter=0;
unsigned char sign,t,he;
float ax[3];//��Ϊϵͳ��ֹʱ�ļ��ٶȲ���
float x=0.1;//��ֵ
int static ct=0;//ͳ����ֵ��Ĵ���
int start=0;
int end =0;//��ֹʱ��
static unsigned char Temp[11];
char tmp[40];
static int runflag=0;//�˶���־λ
static int berunflag=0;
static int quitflag=0;
static int bequitflag=0;
static int quitnum=0;//��ֹ������
int quitsec=2;//��ֹ��2��

void mpu6050_init()
{
	USART2_Config();
  NVIC_Configuration();	
}

void mpu6050_close()
{
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); // ʧ��ָ����USART2�����ж�
}

void mpu6050_open()
{
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART2�����ж�
}

void USART2_IRQHandler(void)		   //����2ȫ���жϷ�����
{
   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж���Ч,���������ݼĴ�����
     {
      Temp[counter] = USART_ReceiveData(USART2);   //��������
      //�������ĳ���
      //if(counter == 0 && Re_buf[0] != 0x55) return;      //�� 0 �����ݲ���֡ͷ������
	  if(counter == 0 && Temp[0] != 0x55) return;      //�� 0 �����ݲ���֡ͷ������
      counter++; 
      if(counter==11) //���յ� 11 ������
      { 
         mymemcpy(Re_buf,Temp,11);
         counter=0; //���¸�ֵ��׼����һ֡���ݵĽ���
         sign=1;
      }			
   }
}

int getdata(void)
{
   unsigned char Temp[11];
	
      if(sign)
      {  
         memcpy(Temp,Re_buf,11);
         sign=0;
         if(Re_buf[0]==0x55)       //���֡ͷ
         {  
            switch(Re_buf[1])
            {
               case 0x51: //��ʶ������Ǽ��ٶȰ�
                  a[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16;      //X����ٶ�
                  a[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16;      //Y����ٶ�
                  a[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16;      //Z����ٶ�
                  T    = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //�¶�
                  break;
               case 0x52: //��ʶ������ǽ��ٶȰ�
                  w[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X����ٶ�
                  w[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y����ٶ�
                  w[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z����ٶ�
                  T    = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //�¶�
                  break;
               case 0x53: //��ʶ������ǽǶȰ�
                  angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
                  angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ
                  angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z��ƫ���ǣ�z �ᣩ
                  T        = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //�¶�

                  //printf("X��Ƕȣ�%.2f   Y��Ƕȣ�%.2f   Z��Ƕȣ�%.2f\r\n",angle[0],angle[1],angle[2]);
                  break;
               default:  break;
            }
			//			printf("X�Ƕȣ�%.2f  Y�Ƕȣ�%.2f  Z�Ƕȣ�%.2f  X�ٶȣ�%.2f  Y�ٶȣ�%.2f  Z�ٶȣ�%.2f\r\n",angle[0],angle[1],angle[2],w[0],w[1],w[2]);
				//		printf("x:%.2f   y:%.2f   z:%.2f\r\n",a[0],a[1],a[2]);
          //ִ�м���㷨
//						if(monitor(ax,a)==1)//�˶�״̬
//					 {
//						 if(runflag==0)
//						 {
//								blsend("running!\r\n");
//								runflag=1;//��1,˵�����͹�run��Ϣ
//							  quitflag=0;//˵��������run״̬�����Է���quit״̬��
//							  bequitflag=0;
//							  berunflag=1;
//						 }
//					 }
//					 else if(monitor(ax,a)==0)//��ֹ״̬
//					 {
//						 if(quitflag==0)
//						 {
//							 blsend("quit!\r\n");
//							 quitflag=1;//quitλ��1��˵�����͹�quit��Ϣ
//							 runflag=0;//�˶�λ��0��˵��������quit״̬�����Է���run״̬��
//							 bequitflag=1;
//							 berunflag=0;
//						 }
//					 }
//					 else if(monitor(ax,a)==-1)//������ֹ״̬
//					{
//						if(bequitflag==0)
//						{
//							 blsend("will quit!\r\n");
//						 	 bequitflag=1;
//							 quitflag=0;
//							 runflag=0;
//							 berunflag=0;
//						}
//					 }
//					else //�����˶�״̬
//					{
//						if(berunflag==0)
//						{
//							blsend("will running!\r\n");
//							 berunflag=0;
//						 	bequitflag=0;
//							 quitflag=1;
//							 runflag=0;
//						}
//					}
//      }
				if(monitorx(ax,a)==1)
				{
					 if(runflag==0)
					 {
							Lora_Printf("running!\r\n");
							runflag=1;//��1,˵�����͹�run��Ϣ
							quitflag=0;//˵��������run״̬�����Է���quit״̬��
					 }
				}
				else
				{
					if(quitflag==0)
					{
				    Lora_Printf("quit!\r\n");
						quitflag=1;//quitλ��1��˵�����͹�quit��Ϣ
					  runflag=0;//�˶�λ��0��˵��������quit״̬�����Է���run״̬��
					}
				}
			}
   }
	 return 0;
}

//����㷨������1�����˶�������0����ֹ ����-1������ֹ������-2�����˶�
//����1 �������飬����2 ʵʱ����
//���ģ���������ٶ�����ֵ��Χ�ڲ���,�ж���ֹ��2��������ֵ��Χ�Ⲩ��������2�Σ��ж��˶�
int monitor(float ax[],float a[])
{
	int i;
	//�״ε��ã���ʼ��ֵ
	if(ax[0]==0&&ax[1]==0&&ax[2]==0)
	{
		for(i=0;i<3;i++)
		{
			ax[i]=a[i];
		}
	}
	//���濪ʼ���
	if( ax[0]-a[0]>x||ax[0]-a[0]<(-x) || ax[1]-a[1]>x||ax[1]-a[1]<(-x)  || ax[2]-a[2]>x||ax[2]-a[2]<(-x))//�����ٶȱ仯������ֵ
	{
			ct++;
			if(ct==1)//ctΪ1˵���״γ��ּ��ٶȲ���
			{
				start=get_time().second;
			}
			else
			{
				end=get_time().second;
			  if(end-start<0)//ʱ���Խ��һ����
			  {
				  end+=60;
			  }
			}
			if(end-start>=0)	
			{	
					if(ct>0)//���ٶȱ仯��1��
					{
					  ct=0;
						start=0;
						end=0;
					for(i=0;i<3;i++){ax[i]=0;}//����
					quitnum=0;//��ֹ��������0
					return 1;
					}
					else
					{
							//������Ҫ�󣬿�����һ�����ѭ��
						ct=0;
						start=0;
						end=0;
					for(i=0;i<3;i++){ax[i]=0;}//����
					quitnum=0;//��ֹ��������0
						return -2;
					}
			}
}
	//���ٶȱ仯û�г�����ֵ
	else
	{
		quitnum++;
		if(quitnum>quitsec*200)//��Լ�Ǿ�ֹ��3����
		{
			quitnum=0;
			return 0;
		}
		
	}
	return -1;
}


int monitorx(float ax[],float a[])
{
		int i;
	//�״ε��ã���ʼ��ֵ
	if(ax[0]==0&&ax[1]==0&&ax[2]==0)
	{
		for(i=0;i<3;i++)
		{
			ax[i]=a[i];
		}
	}
	//���濪ʼ���
	if( ax[0]-a[0]>x||ax[0]-a[0]<(-x) || ax[1]-a[1]>x||ax[1]-a[1]<(-x)  || ax[2]-a[2]>x||ax[2]-a[2]<(-x))//�����ٶȱ仯������ֵ
	{
			ct++;
			if(ct==1)//ctΪ1˵���״γ��ּ��ٶȲ���
			{
				start=get_time().second;
			}
			else
			{
				end=get_time().second;
			  if(end-start<0)//ʱ���Խ��һ����
			  {
				  end+=60;
			  }
			}
			if(end-start>=0)	
			{	
					if(ct>0)//���ٶȱ仯��1��
					{
					  ct=0;
						start=0;
						end=0;
					for(i=0;i<3;i++){ax[i]=0;}//����
					quitnum=0;//��ֹ��������0
					return 1;
					}
					else
					{
							//������Ҫ�󣬿�����һ�����ѭ��
						ct=0;
						start=0;
						end=0;
					for(i=0;i<3;i++){ax[i]=0;}//����
					quitnum=0;//��ֹ��������0
						return 1;
					}
				}
			return 1;
			}
	//���ٶȱ仯û�г�����ֵ
	else
	{
		quitnum++;
		if(quitnum>quitsec*200)//��Լ�Ǿ�ֹ��3����
		{
			return 0;
		}
		return 1;
	}
	return 1;
}


void mymemcpy(unsigned char * Re_buf,unsigned char * Temp,int count)
{
	int i=0;
	for(;i<count;i++)
	{
		Re_buf[i]=Temp[i];
	}
	
}

