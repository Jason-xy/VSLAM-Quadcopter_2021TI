#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "remote_controller.h"
//////////////////////////////////////////////////////////////////////////////////	 

//STM32������
//����1��ʼ��		   

////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1 			1		//ʹ�ܣ�1��/��ֹ��0������1 PA9 PA10
#define EN_USART2	  		1		//����2 PD6 PD5


extern u8  	USART1_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 	USART1_RX_STA;         		//����״̬���
extern u8  	USART2_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 	USART2_RX_STA;         		//����״̬���     00000000  00000000 �߰�λ��ʾ����״̬���ڰ�λ��ʾ


void uart1_init(u32 bound);
void uart2_init(u32 bound);

void uart1_data_ANL(u8* data,u8 len);
void uart2_data_ANL(u8* data,u8 len);

#endif


