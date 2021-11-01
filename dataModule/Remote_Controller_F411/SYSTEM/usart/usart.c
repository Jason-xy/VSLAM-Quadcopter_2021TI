#include "sys.h"
#include "usart.h"	


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif




//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART1_RX_STA=0;       //接收状态标记	

void uart1_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
		 
	u8 data=0;
	u8 i=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		
		data=USART_ReceiveData(USART1);
		//USART_SendData(USART2, data);
		//USART_SendData(USART2,(char)USART1_RX_STA);
		if((USART1_RX_STA)==0 && data==0xAA)//帧头
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			USART1_RX_STA++;
					
		}
		else if((USART1_RX_STA)==1 && (data==0xAF || data==0xFF))//目标地址
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			USART1_RX_STA++;
			
		}
		else if((USART1_RX_STA)==2 &&  data==0xE0)	//功能码00为校验帧A0 为log字符串
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			USART1_RX_STA++;
			
		}
		else if((USART1_RX_STA)==3)//len
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			USART1_RX_STA++;
			
		}
		else if((USART1_RX_STA)>=3 &&(USART1_RX_STA)<=(3+USART1_RX_BUF[3]) && USART1_RX_BUF[3]>0)
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			USART1_RX_STA++;
			
		}
		else if((USART1_RX_STA)==(4+USART1_RX_BUF[3]))//和校验
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			USART1_RX_STA++;
			
		}
		else if((USART1_RX_STA)==(5+USART1_RX_BUF[3]))//附加校验
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			uart1_data_ANL(USART1_RX_BUF,USART1_RX_STA+1);
			USART1_RX_STA=0;
		}
		else
		{
			USART1_RX_STA=0;
			
		}
	}  

} 

  
  
  
//按照这种方式在使用缓冲区之后需要将USART1_RX_STA置为0，才能进行下一次数据的读取



#if EN_USART2   //如果使能了接收
//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART2_RX_STA=0;       //接收状态标记	

void uart2_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART2, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}


//用于接收数传的数据

void USART2_IRQHandler(void)                	//串口1中断服务程序
{

	u8 data=0;
	u8 i=0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		
		data=USART_ReceiveData(USART2);

		if((USART2_RX_STA)==0 && data==0xAA)//帧头
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			USART2_RX_STA++;
					
		}
		else if((USART2_RX_STA)==1 && (data==0xAF || data==0xFF))//目标地址
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			USART2_RX_STA++;
			
		}
		else if((USART2_RX_STA)==2 &&  (data==0x00 || data==0xA0))	//功能码00为校验帧A0 为log字符串
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			USART2_RX_STA++;
			
		}
		else if((USART2_RX_STA)==3)//len
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			USART2_RX_STA++;
			
		}
		else if((USART2_RX_STA)>=3 &&(USART2_RX_STA)<=(3+USART2_RX_BUF[3]) && USART2_RX_BUF[3]>0)
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			USART2_RX_STA++;
			
		}
		else if((USART2_RX_STA)==(4+USART2_RX_BUF[3]))//和校验
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			USART2_RX_STA++;
			
		}
		else if((USART2_RX_STA)==(5+USART2_RX_BUF[3]))//附加校验
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			uart2_data_ANL(USART2_RX_BUF,USART2_RX_STA+1);
			USART2_RX_STA=0;
		}
		else
		{
			USART2_RX_STA=0;
			
		}
	}  
	

	
}





void uart1_data_ANL(u8* data,u8 len){
	//data check
	uint8_t i=0;
	uint8_t check_sum1 = 0, check_sum2 = 0;
	
	for ( i = 0; i < len - 2; i++)
	{
		check_sum1 += *(data + i); //Sum check
		check_sum2 += check_sum1;  //add on check
	}

	if ((check_sum1 != *(data + len - 2)) || (check_sum2 != *(data + len - 1))) //compare the checksum
		return;
	/*================================================================================*/
	USART_SendData(USART2, data[4]);
	USART_SendData(USART2, data[6]);
	if(*(data+4)==0x01)
	{
		if(*(data+6)==0x01)
		{
			sent_CMD_ACC_calibration();
		}
		else if(*(data+6)==0x02)
		{
			sent_CMD_GYRO_calibration();
		}
		else if(*(data+6)==0x03)
		{
			sent_CMD_RH_calibration();
		}
		else if(*(data+6)==0x04)
		{
			sent_CMD_MAG_calibration();
		}
		else if(*(data+6)==0x05)
		{
			sent_CMD_SixSides_calibration();
		}
		
	}
	else if(*(data+4)==0x10)
	{
		
		if(*(data+6)==0x01)
		{
			sent_CMD_unlock();
		}
		else if(*(data+6)==0x02)
		{
			sent_CMD_Estop();
		}
		
	}
	
}
void uart2_data_ANL(u8* data,u8 len){
	//data check
	uint8_t i=0;
	uint8_t check_sum1 = 0, check_sum2 = 0;
	
//	if (*(data + 3) != (len - 6)) //check the length of the data
//		return;
	for (i = 0; i < len - 2; i++)
	{
		check_sum1 += *(data + i); //Sum check
		check_sum2 += check_sum1;  //add on check
	}
	if ((check_sum1 != *(data + len - 2)) || (check_sum2 != *(data + len - 1))) //compare the checksum
		return;
	
	
	/*================================================================================*/
	//Get data
	if(*(data+2)==0x00)//数据
	{
		
	}
	else if(*(data+2)==0xA0)
	{
		sent_utf8_txt(data,len);
	}
	
}

#endif
