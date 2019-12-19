#include "stm32f10x.h"
//#include "lib/types.h"
#include "stm32f10x_gpio.h"
//#include "bluetooth.h"


//PB8 ->ADDO
//PB9 ->ADSK
#define     ADDO_H()     GPIO_SetBits(GPIOB, GPIO_Pin_13);//��1
#define     ADDO_L()     GPIO_ResetBits(GPIOB, GPIO_Pin_13);//��0
#define     ADSK_H()     GPIO_SetBits(GPIOB, GPIO_Pin_14);//��1
#define     ADSK_L()     GPIO_ResetBits(GPIOB, GPIO_Pin_14);//��0

#define     READADDO()   GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define     READADSK()   GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)//��ȡ��Ӧλ�����룬�ڸ�λ����ģʽ��Ч

#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)

#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */


void MYGPIO_Init(void);
unsigned long ReadCount(void);
void HX711_init(void);
void HX711_ADDOIn(void);
void HX711_ADDOOut(void);
void HX711_ADSKIn(void);
void HX711_ADSKOut(void);
void sendpress();
void sendchange();
int get_kg();
