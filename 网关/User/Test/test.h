#ifndef  __TEST_H
#define	 __TEST_H



#include "stm32f10x.h"
#include "bsp_esp8266.h"



/********************************** �û���Ҫ���õĲ���**********************************/
#define      macUser_ESP8266_BulitApSsid                  "iuni u3"         //Ҫ�������ȵ������
#define      macUser_ESP8266_BulitApEcn                   WPA2 PSK        //Ҫ�������ȵ�ļ��ܷ�ʽ
#define      macUser_ESP8266_BulitApPwd                   "iuni u3"         //Ҫ�������ȵ����Կ

#define      macUser_ESP8266_CwjServer_M                 "iuni u3"  //�����ȵ������
#define      macUser_ESP8266_CwjServer_N                 "12345678"  //�����ȵ������




#define      macUser_ESP8266_TcpServer_Enum               enumTCP    //Э������
#define      macUser_ESP8266_TcpServer_IP                 "192.168.2.1"    //������������IP��ַ
#define      macUser_ESP8266_TcpServer_Port               "20002"             //�����������Ķ˿�   

#define      macUser_ESP8266_pStr               "zhangqing"             //�������� 
#define      macUser_ESP8266_ulStrLength         8             //�����ֽ���



#define      macUser_ESP8266_TcpServer_OverTime           "1800"             //��������ʱʱ�䣨��λ���룩



/********************************** ���Ժ������� ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );



#endif

