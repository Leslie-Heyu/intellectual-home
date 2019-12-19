//PB13 -> ADDO �������ת����ɵ��ж��ź�(����ģʽ)������
//PB14 -> ADSK ������߿�ʼ��ȡ����(���ģʽ)ʱ����


#include "stm32f10x.h"
#include "hx711drv.h"
static int kg;

int get_kg()//���ѹ����С
{
		unsigned long adc;
		adc=ReadCount();
		adc/=100000;
		kg=adc-78;
		return kg;
}
void sendchange()//����ѹ���ı侯��
{
			unsigned long adc;
	//		char dis[30];
			adc=ReadCount();
	//			sprintf(dis,"%ld\r\n",adc);
	 //   Lora_Printf(dis);
	//	if(adc>9000000||adc<8500000)
			adc/=100000;
		if(78+kg!=adc&&78-kg!=adc)
		{
		Lora_Printf("pressed have changed!\r\n");
			printf("��ǰadcֵ:%d\r\n",adc);
			printf("��ǰkg:%d\r\n",kg);
	   	kg=adc-78;
			printf("�´βο���kg:%d\r\n",kg);
			
		}
}

void sendpress()//����ѹ��ֵ�Ĵ�Լֵ����λkg
{
	char dis[30];
		sprintf(dis,"pressed is about %.2f kg\r\n",get_kg());
		Lora_Printf(dis);
}

void HX711_init(void)
{
  MYGPIO_Init();
  HX711_ADDOIn();
  HX711_ADSKOut();
}

unsigned long ReadCount(void)
{
  unsigned long Count;
  unsigned char i;
  HX711_init();

  ADSK_L();//���ģʽ���͵�ƽ

  Count=0;
  while(READADDO()); //ADת��δ������ȴ�������ʼ��ȡ
  for (i=0;i<24;i++)
  {
    ADSK_H();
    Count=Count<<1; //�½�����ʱ����Count����һλ���Ҳಹ��
    ADSK_L();
    if(READADDO()) Count++;
  }
  ADSK_H();
  Count=Count^0x800000;//��25�������½�����ʱ��ת������
  ADSK_L();
  return(Count);
}


//��ʼ��IO��
void MYGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;//����ṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//GPIOE�ⲿʱ��ʹ�ܣ��ر���Ҫ������

	//PB13 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ��ͨ�������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure1);//��ʼ��
	
		//PB14 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ��ͨ�������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure1);//��ʼ��
}

//ADDO ����Ϊ����״̬

void HX711_ADDOIn(void)
{
		GPIO_InitTypeDef GPIO_InitStructure1;//����ṹ��
	//PB14 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;//����Ϊ��ͨ��������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure1);//��ʼ��
}

//ADDO ����Ϊ���״̬
void HX711_ADDOOut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;//����ṹ��
	//PB14 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ��ͨ���
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure1);//��ʼ��
}

//ADSK����Ϊ����ģʽ
void HX711_ADSKIn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;//����ṹ��
	//PB14 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;//����Ϊ��ͨ��������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure1);//��ʼ��
}

// ADSK����Ϊ���״̬

void HX711_ADSKOut()
{
	GPIO_InitTypeDef GPIO_InitStructure1;//����ṹ��
	//PB14 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ��ͨ���
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure1);//��ʼ��
}