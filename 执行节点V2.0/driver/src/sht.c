//P0.1 -->  PC1(SDA)
//P1.7 -->  PD7(SCL)

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "sht.h"
//#include "dev/simple-gpio.h"
#include "stm32f10x_gpio.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
//#define     SDA_H(port)     simple_gpio_output(port->sda_gpio, port->sda_gpio_pin, 1)
//#define     SDA_L(port)     simple_gpio_output(port->sda_gpio, port->sda_gpio_pin, 0)
//#define     SCK_H(port)     simple_gpio_output(port->scl_gpio, port->scl_gpio_pin, 1)
//#define     SCK_L(port)     simple_gpio_output(port->scl_gpio, port->scl_gpio_pin, 0)

#define     SDA_H(port)     GPIO_SetBits(GPIOC, GPIO_Pin_1);//��1
#define     SDA_L(port)     GPIO_ResetBits(GPIOC, GPIO_Pin_1);//��0
#define     SCK_H(port)     GPIO_SetBits(GPIOD, GPIO_Pin_7);//��1
#define     SCK_L(port)     GPIO_ResetBits(GPIOD, GPIO_Pin_7);//��0

//��SDA����
//#define     SDA_R(port)     simple_gpio_read_input(port->sda_gpio, port->sda_gpio_pin)

#define      SDA_R(port)  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)//��ȡ��Ӧλ�����룬�ڸ�λ����ģʽ��Ч
/* Private macro -------------------------------------------------------------*/
#define noACK 0         //��Ӧ��
#define ACK   1         //Ӧ��
#define STATUS_REG_W    0x06   //000   0011    0
#define STATUS_REG_R    0x07   //000   0011    1
#define MEASURE_TEMP    0x03   //000   0001    1
#define MEASURE_HUMI    0x05   //000   0010    1
#define RESET           0x1e   //000   1111    0
/* Private variables ---------------------------------------------------------*/
enum
{TEMP, HUMI
};

/* Private function prototypes -----------------------------------------------*/
void SHT10_Config(sht_port_t *port);
void SHT10_SDAIn(sht_port_t *port);
void SHT10_SDAOut(sht_port_t *port);
void SHT10_Delayus(uint16_t);
uint8_t SHT10_WriteByte(sht_port_t *port, uint8_t value);
uint8_t SHT10_ReadByte(sht_port_t *port, uint8_t Ack);
void SHT10_Start(sht_port_t *port);
void SHT10_ConReset(sht_port_t *port);
uint8_t SHT10_Measure(sht_port_t *port, uint16_t* pValue, uint8_t* pCheckSum, uint8_t mode);
void SHT10_Cal(uint16_t Temp, uint16_t Hum, float* pTempValue, float* pHumValue);

/* Private functions ---------------------------------------------------------*/

			sht_port_t sht1_port =
{
        GPIOD,
        GPIO_Pin_7,
        GPIOC,
        GPIO_Pin_1,
};

static void Delay_ms(uint16_t time)
{
    uint16_t i = 0;
    while (time--)
    {
        i = 12000;  //�Լ�����
        while (i--)
            ;
    }
}

float get_temp()
{
	float temp;
	float humi;
		sht_read(&sht1_port,&temp,&humi);
	return temp;
}

float get_humi()
{
		float temp;
	float humi;
		sht_read(&sht1_port,&temp,&humi);
	return humi;
}
//lora��������
void sendsht()
{
	char dis[60];
	float temp=29.52;
	float humi= 78.14;
//	sprintf(dis,"temp:%.2f\thumi:%.2f\r\n",get_temp(),get_humi());
	sprintf(dis,"temp:%.2f\thumi:%.2f\r\n",temp,humi);
	Lora_Printf(dis);
}
uint8_t sht_read(sht_port_t *port, float *temp, float *humi)
{
    //�¶Ƚ��  16bit
    uint16_t TempValue;
    //ʪ�Ƚ�� 16bit
    uint16_t HumValue;
    //�¶�ת�����
    float TempResult;
    //ʪ��ת�����
    float HumResult;
    //У��ֵ
    uint8_t CheckValue = 0x00;

    //����
    uint8_t error = 0x00;

    SHT10_Config(port);

    //SHT10 ����
    SHT10_ConReset(port);

    //����¶Ⱥ�ʪ�����ݣ�16λ��ʽ
    error += SHT10_Measure(port, &TempValue, &CheckValue, TEMP);
    error += SHT10_Measure(port, &HumValue, &CheckValue, HUMI);
    //�¶�ʪ�ȼ��㣬��������ʽ
    SHT10_Cal(TempValue, HumValue, &TempResult, &HumResult);
    //ͨ������������¶Ⱥ�ʪ������
    if (!error)
    {
        *temp = TempResult;
        *humi = HumResult;//��������Ӳ�����󣬿����ڴ�Ƿ�������ɣ�
    }

    return error;

}

/**
 * @brief  ��ʼ��SHT10 IO��
 * @param  None
 * @retval None
 */
void SHT10_Config(sht_port_t * port)
{
		GPIO_InitTypeDef GPIO_InitStructure1;//����ṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);//GPIOC D�ⲿʱ��ʹ�ܣ��ر���Ҫ������
//    simple_gpio_makeoutput(port->scl_gpio, port->scl_gpio_pin,
//            GPIO_Mode_Out_PP);
//    simple_gpio_makeoutput(port->sda_gpio, port->sda_gpio_pin,
//            GPIO_Mode_Out_PP);

	//PC1 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ��ͨ�������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure1);//��ʼ��
	
		//PD7 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;//����Ϊ��ͨ�������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure1);//��ʼ��
}

/**
 * @brief  ����Ϊ����״̬
 * @param  None
 * @retval None PC1(SDA)
 */
void SHT10_SDAIn(sht_port_t * port)
{
//    simple_gpio_makeinput(port->sda_gpio, port->sda_gpio_pin,
//            GPIO_Mode_IN_FLOATING);
		GPIO_InitTypeDef GPIO_InitStructure1;//����ṹ��
	//PC1 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;//����Ϊ��ͨ��������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure1);//��ʼ��
}

/**
 * @brief  ����Ϊ���״̬
 * @param  None
 * @retval None
 */
void SHT10_SDAOut(sht_port_t * port)
{
//    simple_gpio_makeinput(port->sda_gpio, port->sda_gpio_pin, GPIO_Mode_Out_PP);
			GPIO_InitTypeDef GPIO_InitStructure1;//����ṹ��
	//PC1 config
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure1.GPIO_Mode =  GPIO_Mode_Out_PP;//����Ϊ��ͨ�������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure1);//��ʼ��
}

/**
 * @brief  ����Ϊ���״̬
 * @param  д����
 * @retval Ӧ��
 */
uint8_t SHT10_WriteByte(sht_port_t *port, uint8_t value)
{
    uint8_t i, error = 0;
    //SDA���
    SHT10_SDAOut(port);

    for (i = 0x80; i > 0; i /= 2)
    {
        if (i & value)
       //     SDA_H(port);
				GPIO_SetBits(GPIOC, GPIO_Pin_1);
        else
            SDA_L(port);
        //SHT10_Delayus(1);
        SCK_H(port);
        SHT10_Delayus(2);
        SCK_L(port);
        SHT10_Delayus(1);
    }

    SDA_H(port);
    SCK_H(port);
    //SDA����
    SHT10_SDAIn(port);
    error = SDA_R(port);   //��Ӧ��λ
    SCK_L(port);

    return error;
}

/**
 * @brief  ������
 * @param  Ӧ��
 * @retval ��������
 */
uint8_t SHT10_ReadByte(sht_port_t *port, uint8_t Ack)
{
    uint8_t i, val = 0;

    SHT10_SDAOut(port);
    SDA_H(port);
    //����״̬
    SHT10_SDAIn(port);

    for (i = 0x80; i > 0; i /= 2)
    {
        SCK_H(port);
        SHT10_Delayus(1);
        if (SDA_R(port))
            val = (val | i);        //������
        SCK_L(port);
    }

    //���״̬
    SHT10_SDAOut(port);
    if (Ack)
    //    SDA_L(port);                //Ӧ��Ϊ�͵�ƽ
		 GPIO_ResetBits(GPIOC, GPIO_Pin_1);
    else
        SDA_H(port);

    SCK_H(port);
    SHT10_Delayus(10);
    SCK_L(port);
    SDA_H(port);
    return val;
}

/**
 * @brief  ����
 * @param  ��
 * @retval ��
 */
void SHT10_Start(sht_port_t *port)
{
    //SDA���
    SHT10_SDAOut(port);

    SDA_H(port);
    SCK_L(port);
    SHT10_Delayus(1);
    SCK_H(port);
    SHT10_Delayus(1);
    SDA_L(port);
    SHT10_Delayus(1);
    SCK_L(port);
    SHT10_Delayus(3);
    SCK_H(port);
    SHT10_Delayus(1);
    SDA_H(port);
    SHT10_Delayus(1);
    SCK_L(port);

}

/**
 * @brief  ��������
 * @param  ��
 * @retval ��
 */
void SHT10_ConReset(sht_port_t *port)
{
    uint8_t i;
    //���
    SHT10_SDAOut(port);

    SDA_H(port);    //����ߵ�ƽ
    SCK_L(port);

    for (i = 0; i < 9; i++)
    {
        SCK_H(port);
        SHT10_Delayus(1);
        SCK_L(port);
        SHT10_Delayus(1);
    }

    SHT10_Start(port);
}

/**
 * @brief  �������
 * @param  ��
 * @retval ��
 */
uint8_t SHT10_SoftReset(sht_port_t *port)
{
    uint8_t error = 0;
    SHT10_ConReset(port);
    error += SHT10_WriteByte(port, RESET);
    return error;
}

/**
 * @brief  �¶Ȼ�ʪ�Ȳ���
 * @param  �¶Ȼ���ʪ��ָ�����ݣ�У��ֵָ�룬ģʽ
 * @retval ����
 */
uint8_t SHT10_Measure(sht_port_t *port, uint16_t* pValue, uint8_t* pCheckSum,
        uint8_t mode)
{
    uint8_t error = 0;

    uint8_t Value_H = 0;
    uint8_t Value_L = 0;

    uint16_t i = 0;

    //����
    SHT10_Start(port);
    switch (mode)
    {
    case TEMP:
        error += SHT10_WriteByte(port, MEASURE_TEMP);
        break;
    case HUMI:
        error += SHT10_WriteByte(port, MEASURE_HUMI);
        break;
    default:
        break;
    }

    if (error > 0)
        return error;

    //SDA��״̬
    SHT10_SDAIn(port);
    while (SDA_R(port))
    {
        Delay_ms(1);
        if (i >= 500)
        {
            error += 1;
            return error;
        }
        i++;

    }

    Value_H = SHT10_ReadByte(port, ACK);    //����λ
    Value_L = SHT10_ReadByte(port, ACK);    //����λ

    *pCheckSum = SHT10_ReadByte(port, noACK);  //��У����

    //���ؽ��
    *pValue = (Value_H << 8) | Value_L;

    return error;
}

/**
 * @brief  �����¶Ⱥ�ʪ������
 * @param  �¶����� ʪ������ �¶Ƚ�� ʪ�Ƚ��
 * @retval ��
 */
void SHT10_Cal(uint16_t Temp, uint16_t Hum, float* pTempValue, float* pHumValue)
{
    const float d1 = -39.66;
    const float d2 = 0.01;
	
	    const float C1 = -4.0;
    const float C2 = +0.0405;
    const float C3 = -2.8E-6;
    const float T1 = +0.01;
    const float T2 = +0.00008;
	
    float Temp_C;

    //ʪ������ֵ
    float RH_Lin;
    //ʪ����ʵֵ
    float RH_True;

	    //�¶Ƚ��������
    Temp_C = d1 + d2 * Temp;
	
    //RH���Խ��
    RH_Lin = C1 + C2 * Hum + C3 * Hum * Hum;
    RH_True = (Temp_C - 25) * (T1 + T2 * Hum) + RH_Lin;
    //�޶���Χ
    if (RH_True > 100)
        RH_True = 100;
    if (RH_True < 0.01)
        RH_True = 0.01;

    *pTempValue = Temp_C;
    *pHumValue = RH_True;

}

/**+
 * @brief  ��ʱ����
 * @param  ��
 * @retval ��
 */
void SHT10_Delayus(uint16_t time)
{
    uint16_t i = 0;
    while (time--)
    {
        i = 10;  //�Լ�����
        while (i--)
            ;
    }
}

void SHT10_Delayms(uint16_t time)
{
    uint16_t i = 0;
    while (time--)
    {
        i = 12000;  //�Լ�����
        while (i--)
            ;
    }
}
