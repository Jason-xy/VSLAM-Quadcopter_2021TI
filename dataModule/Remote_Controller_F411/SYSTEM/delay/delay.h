#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	  
//////////////////////////////////////////////////////////////////////////////////  

// STM32F407������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��ucosii)
//����delay_us,delay_ms

////////////////////////////////////////////////////////////////////////////////// 	 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























