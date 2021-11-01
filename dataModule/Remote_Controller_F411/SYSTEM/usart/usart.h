#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "remote_controller.h"
//////////////////////////////////////////////////////////////////////////////////	 

//STM32开发板
//串口1初始化		   

////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1 			1		//使能（1）/禁止（0）串口1 PA9 PA10
#define EN_USART2	  		1		//串口2 PD6 PD5


extern u8  	USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 	USART1_RX_STA;         		//接收状态标记
extern u8  	USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 	USART2_RX_STA;         		//接收状态标记     00000000  00000000 高八位表示接收状态，第八位表示


void uart1_init(u32 bound);
void uart2_init(u32 bound);

void uart1_data_ANL(u8* data,u8 len);
void uart2_data_ANL(u8* data,u8 len);

#endif


