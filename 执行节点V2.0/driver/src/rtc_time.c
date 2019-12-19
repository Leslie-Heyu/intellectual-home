#include "rtc_time.h"
#include "stm32f10x.h"



#define  BKP_TIME_FLAG            BKP_DR1
#define  BKP_TIME_YEAR            BKP_DR2
#define  BKP_TIME_MONTH           BKP_DR3
#define  BKP_TIME_DAY             BKP_DR4
#define  BKP_TIME_DATE            BKP_DR5
static rtc_time_t Real_Time;


const uint8_t TAB_DATE[12]={6,2,2,5,0,3,5,1,4,6,2,4,};


rtc_time_t get_time(void)
{
    return Real_Time;
}

uint8_t get_date(rtc_time_t* time)
{
        return( (time->year + time->year/4 - ( (time->month<3)&&(time->year%4==0) ) + TAB_DATE[time->month-1] + time->day )%7);
}


const uint8_t Month2Day_Tab[12]={31,28,31,30,31,30,31,31,30,31,30,31};
void rtc_to_time(void)
{
    uint32_t count;
    uint8_t tmp,change=0;
    Real_Time.year=BKP_ReadBackupRegister(BKP_TIME_YEAR);//��ֵ
    Real_Time.month=BKP_ReadBackupRegister(BKP_TIME_MONTH);//��ֵ
    Real_Time.day=BKP_ReadBackupRegister(BKP_TIME_DAY);//��ֵ
    Real_Time.date=BKP_ReadBackupRegister(BKP_TIME_DATE);//����ֵ

    RTC_ITConfig(RTC_IT_SEC, DISABLE);        //Ϊ�˱�������������������,�����RTC���ж�������
    count=RTC_GetCounter();
    //�����µ�������
    while (count>=0x0001517f)         //�ϴιص絽���ο�Խ��һ������
    {
        change=1;
        count-=0x0001517f;
        //�����Լ�
        if ((++Real_Time.date)>=8)
        {
            Real_Time.date=1;
        }

        //�����2�£���������(����Ҫ����2099�Ժ��400��һ����
        if (Real_Time.month==2)
        {
            if (Real_Time.year%4)
                    tmp=28;
            else
                    tmp=29;
        }
        else
        {
            tmp=Month2Day_Tab[Real_Time.month-1];
        }
        if ((++Real_Time.day)>tmp)
        {
            Real_Time.day=1;

            if ((++Real_Time.month)>12)
            {
                Real_Time.month=1;

                if ((++Real_Time.year)>=100)
                {
                        Real_Time.year=0;
                }
            }
        }
    }
    //�����µ�ʱ����
    Real_Time.hour=count/3600;
    Real_Time.minute=(count%3600)/60;
    Real_Time.second=(count%3600)%60;

    //�����Խ��һ�죬������Ҫ��ؼĴ�����
    if (change)
    {
        RTC_SetCounter(count);
        BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
        BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
        BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
        BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
    }
    //���´�RTC�ж�
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
}

//����rtcʱ��
void set_rtc_time(rtc_time_t* time)
{
    uint32_t count;
    RTC_ITConfig(RTC_IT_SEC, DISABLE);        //�ر����ж�
    RTC_WaitForLastTask();
    //��ʱ��ֵ��Real_Time��
    Real_Time.year=time->year;
    Real_Time.month=time->month;
    Real_Time.day=time->day;
    Real_Time.hour=time->hour;
    Real_Time.minute=time->minute;
    Real_Time.second=time->second;
    //��������
    time->date=Real_Time.date=get_date(time);
    //���µ������մ浽����Ĵ�����
    BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
    //        RTC_WaitForLastTask();
    BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
    //        RTC_WaitForLastTask();
    BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
    //        RTC_WaitForLastTask();
    BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
    //        RTC_WaitForLastTask();
    //�����µ�RTC countֵ
    count=Real_Time.hour*3600+Real_Time.minute*60+Real_Time.second;
    RTC_WaitForLastTask();
    RTC_SetCounter(count);
    // Check if the Power On Reset flag is set
    //ע���ɫ���벿�֣�������������䣬�޸�ʱ��һֱ������        RTC_WaitForLastTask()
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);  

    PWR_BackupAccessCmd(ENABLE);

    RTC_WaitForLastTask();
    RTC_ITConfig(RTC_IT_SEC, ENABLE); //�����ж�
}



void rtc_configuration(void)
{
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);

    BKP_DeInit();

    RCC_LSEConfig(RCC_LSE_ON);

    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {} 

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    RCC_RTCCLKCmd(ENABLE);

    RTC_WaitForSynchro();

    RTC_WaitForLastTask();

    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    RTC_WaitForLastTask();

    RTC_SetPrescaler(32767);

    RTC_WaitForLastTask();
}

void nvic_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //ʹ��RTC�жϣ����������ȼ� 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//��ռ���ȼ�2λ��2λ
    NVIC_InitStructure.NVIC_IRQChannel =RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//USART1_IRQChannel_Priority; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}

void rtc_init(void)
{
    //���ж�
    nvic_init();
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    
    PWR_BackupAccessCmd(ENABLE);
 
    if(BKP_ReadBackupRegister(BKP_TIME_FLAG) != 0xA5A5)
    {

        rtc_configuration();

        Real_Time.year = 2014;
        Real_Time.month = 1;
        Real_Time.day = 1;
        Real_Time.hour = 12;
        Real_Time.minute = 0;
        Real_Time.second = 0;
        set_rtc_time(&Real_Time);
        BKP_WriteBackupRegister(BKP_TIME_FLAG, 0xA5A5);
    }
    else
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
        PWR_BackupAccessCmd(ENABLE);
        BKP_TamperPinCmd(DISABLE);
        BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
        rtc_to_time();
    }
    RTC_WaitForSynchro();
    RCC_ClearFlag();  //���RCC������־
}

void RTC_IRQHandler(void)  
{  
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)  
  {  
  
    /* Clear the RTC Second interrupt */  
    RTC_ClearITPendingBit(RTC_IT_SEC);     
    
    /* Wait until last write operation on RTC registers has finished */  
    RTC_WaitForLastTask();  
    
    rtc_to_time();
      
  }  
} 