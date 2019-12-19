#include "contiki.h"
#include "usart.h"

static process_event_t event_data_ready;

 /*������������*/
 PROCESS(print_hello_process, "Hello");
 PROCESS(print_world_process, "world");
AUTOSTART_PROCESSES(&print_hello_process, &print_world_process); //�ø�������������
 /*������� print_hello_process*/
 PROCESS_THREAD(print_hello_process, ev, data)
 {
	 static struct etimer timer;
	 	 	static int testnum=0;
 PROCESS_BEGIN();
 etimer_set(&timer, CLOCK_CONF_SECOND); //#define CLOCK_CONF_SECOND 10 �� timer �� interval ��Ϊ 1 �����

 printf("***print hello process start***\n");

 event_data_ready = process_alloc_event(); //return lastevent++; �½�һ���¼�����ʵ������һ�� unsigned charֵ����ʶ��ͬ�¼�

 while (1)
 {
 PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER); //����� 4.2���� etimer ���ڼ���ִ����������

// printf("Hello\n");

	 testnum++;
 process_post(&print_world_process, event_data_ready, &testnum); //�����첽�¼��� print_world_process��ֱ���ں˵��ȸý��̲Ŵ�����¼�������� 4.3

 etimer_reset(&timer); //���ö�ʱ��������� 4.4
 }

 PROCESS_END();
 }
 /*������� print_world_process*/

 PROCESS_THREAD(print_world_process, ev, data)
 {
 PROCESS_BEGIN();

 printf("***print world process start***\n");

 while (1)
 {
 PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);
 //printf("world\n");
	 printf("%d\n",(*(int *)data));
 }

 PROCESS_END();
}