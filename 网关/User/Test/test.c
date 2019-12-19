#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "lora.h"

uint8_t ESP8266_CWLIF ( char * pStaIp )//��ȡ���ӵ�softap��station��Ϣ
{
	uint8_t uc, ucLen;
	
	char * pCh, * pCh1;
	
	
  ESP8266_Cmd ( "AT+CWLIF", "OK", 0, 100 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "," );
	
	if ( pCh )
	{
		pCh1 = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "AT+CWLIF\r\r\n" ) + 11;
	  ucLen = pCh - pCh1;
	}

	else
		return 0;
	
	for ( uc = 0; uc < ucLen; uc ++ )
		pStaIp [ uc ] = * ( pCh1 + uc);
	
	pStaIp [ ucLen ] = '\0';
	
	return 1;
	
}



uint8_t ESP8266_CIPAP ( char * pApIp )//����softap��ip��ַ
{
	char cCmd [ 30 ];
		
	
	sprintf ( cCmd, "AT+CIPAP=\"%s\"", pApIp );
	
  if ( ESP8266_Cmd ( cCmd, "OK", 0, 5000 ) )
		return 1;
 
	else 
		return 0;
	
}



/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucId;
	char cStr [ 100 ], cCh;

  char * pCh;
	
	char *CmdOk="�ѷ��͵�Զ���豸!\r\n";
	char *SumtOk="send to server OK!\r\n";
	macESP8266_CH_ENABLE();
	
	//����ģ�飬����wifi�Ĺ���ģʽ
	ESP8266_AT_Test ();    //����AT����,������������
	
	ESP8266_Net_Mode_Choose ( STA );   //stationģʽbool  ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
	
  while ( ! ESP8266_JoinAP ( macUser_ESP8266_CwjServer_M , macUser_ESP8266_CwjServer_N) );//����WiFi�ȵ�

	while ( ! ESP8266_Enable_MultipleId( ENABLE ) );//����ģ��Ϊtcp������ģʽ
	
 // while ( ! ESP8266_Link_Server ( macUser_ESP8266_TcpServer_Enum, macUser_ESP8266_TcpServer_IP,macUser_ESP8266_TcpServer_Port,Single_ID_0 ) );//���ӷ�����	
	
	//printf("ģ����·�����ϵ�ID:%d\r\n",ESP8266_Get_IdLinkStatus());//�˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1���Id���������ӣ�������0���Idδ��������
	
	//����������ģ��Ϊtcp������
	
	  while(!ESP8266_StartOrShutServer ( ENABLE, "8080", "100"));//����һ��tcp��������8080�˿�����������Զ��tcp�ͻ��˷�����ָ��
		
		
	while(! ESP8266_Link_Server ( enumTCP, "106.15.196.133", "8080" , Multiple_ID_1));//����һ��tcp�ͻ��ˣ�����Զ�˷�����,��������,id��Multiple_ID_1
  //	while(!ESP8266_UnvarnishSend())//����͸��ģʽ
		
	while ( 1 )
	{		
		//����wifi�Ļ����ַ�����
		//��������pc,���·��������ֺ�����,��Ҫת����Զ��lora
		//ת����ɺ󣬸�pc��ִ
		if ( strEsp8266_Fram_Record .InfBit .FramFinishFlag )
		{
			int i=0;
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, DISABLE ); //ʧ�ܴ��ڽ����ж�
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';//֡β����0x00,Ϊ�˶Ͽ��ַ���
			
			
			if ( ( pCh = strstr ( (strEsp8266_Fram_Record .Data_RX_BUF+1), "+IPD," ) ) != 0 ) //��wifi�������͵���Ϣ
			{
				printf("pc�·�ָ��/���ݵ���۽ڵ㣬����:%s\r\n",pCh+9);
				//ͨ��loraת����ȥ
			  Lora_Printf(pCh+9);
				
				ESP8266_SendString (DISABLE, CmdOk, strlen(CmdOk), Multiple_ID_1 );//���������ͻ�ִ
			}			
		  strEsp8266_Fram_Record .InfBit .FramLength = 0;//֡����֡������־����0���´δ�buf��0����ʼд
	    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
			
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //ʹ�ܴ��ڽ����ж�
			
		}
		//����lora�ݽ��������ַ���
		if(Lora_Record .InfBit.FramFinishFlag)	
		{
			int i=0;
			USART_ITConfig ( USART2, USART_IT_RXNE, DISABLE ); //ʧ�ܴ��ڽ����жϣ�lora����
			Lora_Record .Data_RX_BUF [ Lora_Record .InfBit .FramLength ]  = '\0';//֡β����0x00,Ϊ�˶Ͽ��ַ���
			
			//lora�ݽ��������ݣ��ݽ���wifiģ�飬��pc
			
			printf("get remote lora's message:%s\r\n",Lora_Record .Data_RX_BUF);
			printf("submit to server...\r\n");
//			if(ESP8266_Get_LinkStatus()!=3)// * ����  : 2�����ip      3����������  4��ʧȥ����     0����ȡ״̬ʧ��
//			{
//				while(! ESP8266_Link_Server ( enumTCP, "106.15.196.133", "8080" , Multiple_ID_1));//����һ��tcp�ͻ��ˣ�����Զ�˷�����,��������,id��Multiple_ID_1
//			}
			
			if(ESP8266_SendString (DISABLE, Lora_Record .Data_RX_BUF, strlen(Lora_Record .Data_RX_BUF), Multiple_ID_1 ));//����wifi
			{
				//lora������������Ϣ�ɹ������ͻ�ִ
			//	Lora_Printf(SumtOk);
			}
			
			Lora_Record .InfBit .FramLength = 0;//֡����֡������־����0���´δ�buf��0����ʼд
	    Lora_Record .InfBit .FramFinishFlag = 0;	
			
			USART_ITConfig ( USART2, USART_IT_RXNE, ENABLE ); //ʹ�ܴ��ڽ����ж�
		}
	}	
}


