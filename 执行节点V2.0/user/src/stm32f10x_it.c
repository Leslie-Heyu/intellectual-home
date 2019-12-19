/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"
#include "sys.h"  
#include "stm32f10x_usart.h"
#include "string.h"
#include "delay.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 



//��Ҫ���ô��ڽ����жϺͶ�ʱ��3�жϣ��ж�ʱ��Ϊ1ms
//------------------------------------------------------------------
//��������void USART1_IRQHandler(void)
//���������null
//���ز�����null
//˵�������ڽ����жϷ���
//------------------------------------------------------------------
//void USART1_IRQHandler(void)
//{
//   u8 rx_data;
//  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)          //�ж϶��Ĵ����Ƿ�ǿ�
//  {      
////    GPIO_SetBits(GPIOB,GPIO_Pin_6);
//    rx_data[RbufCounter++]=USART_ReceiveData(USART1);    //�����ֽڵ����ջ�����
//    if(USART_Rsv_Status==0)
//    {
//      if(RbufCounter>1)
//      {
//        if(rx_data[0]==0xA5&&rx_data[1]==0x5A)    //�����յ�������֡ͷ�����ֽ�ͬʱΪ0xA5��0x5Aʱ
//        {
//          USART_Rsv_Status=1;
////           USART_SendData(USART1, rx_data[0]);
//        }
//        else
//        {
//          rx_data[0]=rx_data[1];
//          RbufCounter=1;
//         
//        }
//      }
//    }
//    else
//    {
//      USART_1ms_Cnt=0;
//    }               
//  }         
//}

//���Է��ͺͽ���һ�δ�������
//void USART1_IRQHandler(u8 GetData)
//{ 
//u8 BackData;
//if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�жϲ���
//{  
//			USART_ClearITPendingBit(USART1,USART_IT_RXNE); //����жϱ�־.
//			GetData=USART1->DR;  	
//		//	GetData = UART1_GetByte(BackData);   //Ҳ��GetData=USART1->DR;   
//			printf("%d",GetData);
//	//	USART1_SendByte(GetData);      //��������
//		
//			LED_ON();
//			delay_ms(1000);
//			LED_OFF();
//} 
//}

//���Է��ͺͽ���һ����ȷ����
//void USART1_IRQHandler() 
// { 
//		u8 RX_dat;                                //�����ַ�����
//    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�жϷ��������ж�
//		{
//   RX_dat=(USART_ReceiveData(USART1) & 0x7F);      //�������ݣ������ȥǰ��λ                                     
//   USART_ClearITPendingBit(USART1,  USART_IT_RXNE);          //����жϱ�־
//		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){}//�ȴ����ս���
////   USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);          //���ж�
//       USART_SendData(USART1,RX_dat);             //��������
//		}
//			LED_ON();      //LED M
//			delay_ms(1000);
//			LED_OFF();
//}


//------------------------------------------------------------------
//��������void TIM2_IRQHandler(void)
//���������null
//���ز�����null
//˵������ʱ��2�жϷ���
//------------------------------------------------------------------
void TIM2_IRQHandler(void)
{
 
}
//------------------------------------------------------------------
//��������void TIM3_IRQHandler(void)
//���������null
//���ز�����null
//˵������ʱ��3�жϷ���
//------------------------------------------------------------------
//void TIM3_IRQHandler(void)
//{ 
//  if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)        //�ж��Ƿ�Ϊ��ʱ��3����ж�
//  {
//    
//    GPIO_SetBits(GPIOB,GPIO_Pin_6);
//    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���жϱ��
//    if(USART_Rsv_Status==1)
//    USART_1ms_Cnt++;
//    if(USART_1ms_Cnt>5)     
//    {
////      USART_SendData(USART1,0xAA);
//      USART_Rsv_Status=0;     //������������5�ζ�USART_Rsv_Status��0�������ȴ�����
//      USART_1ms_Cnt=0;         //��USART_1ms_Cnt>5ʱ��USART_1ms_Cnt�������� 
//      if(RbufCounter==(u16)rx_data[4]+7)              //�������ݵ�������
//      {
//         int i;     //����ѭ������
//        int j;
//        data_length=rx_data[4];
//        for(i=0;i
//        {
//          data[i]=rx_data[i];
//        } 
//        CRC_data_Hi=rx_data[RbufCounter-1];
//        CRC_data_Lo=rx_data[RbufCounter-2];
//        CRC_data=CRC16((unsigned char*)data,data_length+5);
//        CRC_data_Hi1=CRC_data>>8;
//        CRC_data_Lo1=CRC_data&0x00ff;
//         if(CRC_data_Hi==(u8)CRC_data_Hi1 && CRC_data_Lo==CRC_data_Lo1)
//         {
//           for(j=0;rx_data[j]!='\0';j++)   //ѭ�������������������'\0'
//           {       
//             USART_SendData(USART1, rx_data[j]);     //�����ַ�
//             while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//             {
//             } //�ȴ��ַ��������
//           }
//        }
//      }
//      RbufCounter=0;
//    }     
//  }
//}

//void USART3_IRQHandler(void)		   //����3ȫ���жϷ�����
//{
//   if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж���Ч,���������ݼĴ�����
//     {
//      Temp[counter] = USART_ReceiveData(USART3);   //��������
//      //�������ĳ���
//      //if(counter == 0 && Re_buf[0] != 0x55) return;      //�� 0 �����ݲ���֡ͷ������
//	  if(counter == 0 && Temp[0] != 0x55) return;      //�� 0 �����ݲ���֡ͷ������
//      counter++; 
//      if(counter==11) //���յ� 11 ������
//      { 
//         mymemcpy(Re_buf,Temp,11);
//         counter=0; //���¸�ֵ��׼����һ֡���ݵĽ���
//         sign=1;
//      }			
//   }
//}
//void USART3_IRQHandler(void)		   //����3ȫ���жϷ�����
//{
//   if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж���Ч,���������ݼĴ�����
//     {
//      Temp[counter] = USART_ReceiveData(USART3);   //��������
//			 printf("%d",Temp[counter]);
//     }			
//}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
