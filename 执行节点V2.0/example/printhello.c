#include "contiki.h"
#include "usart.h"
#include "led.h"
#include "stdio.h"
#include "hx711drv.h"
#include "6050.h"
#include "sys.h"
#include "rtc_time.h"
#include "bluetooth.h"
#include "sht.h"

PROCESS(tcp_keep, "tcp�������,��ʱ����");//60sһ��

PROCESS(mpu6050, "��������߳�,��̨����");//��������

PROCESS(hx711, "ѹ������̣߳���̨����");//��������

PROCESS(sht_send, "��ʪ�ȶ�ʱ�����߳�");//30sһ��

AUTOSTART_PROCESSES(&tcp_keep,&mpu6050,&hx711,&sht_send);


PROCESS_THREAD(tcp_keep, ev, data)
{
	  static struct etimer et;
    PROCESS_BEGIN();
	while(1)
	{
		etimer_set(&et, CLOCK_SECOND*60);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		Lora_Printf("hello!\r\n");
	}
   PROCESS_END();
}

PROCESS_THREAD(mpu6050, ev, data)
{
	   static struct etimer et;
		PROCESS_BEGIN();
while(1)
{
		etimer_set(&et, CLOCK_SECOND/200);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		getdata();
}
   PROCESS_END();
}

PROCESS_THREAD(hx711, ev, data)
{
	static struct etimer et;
  PROCESS_BEGIN();
		while(1)
		{
		sendchange();//��ѹ���ı䣬���;���
		etimer_set(&et, CLOCK_SECOND/5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		}
   PROCESS_END();
}

PROCESS_THREAD(sht_send, ev, data)
{
	static struct etimer et;
  PROCESS_BEGIN();
		while(1)
		{
		sendsht();//�ύ��ʪ����Ϣ
		etimer_set(&et, CLOCK_SECOND*30);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		}
   PROCESS_END();
}


