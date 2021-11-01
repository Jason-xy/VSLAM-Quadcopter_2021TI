#include "sys.h"
#include "usart.h"	


//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif




//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART1_RX_STA=0;       //����״̬���	

void uart1_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
		 
	u8 data=0;
	u8 i=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		
		data=USART_ReceiveData(USART1);
		//USART_SendData(USART2, data);
		//USART_SendData(USART2,(char)USART1_RX_STA);
		if((USART1_RX_STA)==0 && data==0xAA)//֡ͷ
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			USART1_RX_STA++;
					
		}
		else if((USART1_RX_STA)==1 && (data==0xAF || data==0xFF))//Ŀ���ַ
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			USART1_RX_STA++;
			
		}
		else if((USART1_RX_STA)==2 &&  data==0xE0)	//������00ΪУ��֡A0 Ϊlog�ַ���
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
		else if((USART1_RX_STA)==(4+USART1_RX_BUF[3]))//��У��
		{
			USART1_RX_BUF[USART1_RX_STA&0x00FF]=data;
			USART1_RX_STA++;
			
		}
		else if((USART1_RX_STA)==(5+USART1_RX_BUF[3]))//����У��
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

  
  
  
//�������ַ�ʽ��ʹ�û�����֮����Ҫ��USART1_RX_STA��Ϊ0�����ܽ�����һ�����ݵĶ�ȡ



#if EN_USART2   //���ʹ���˽���
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART2_RX_STA=0;       //����״̬���	

void uart2_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}


//���ڽ�������������

void USART2_IRQHandler(void)                	//����1�жϷ������
{

	u8 data=0;
	u8 i=0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		
		data=USART_ReceiveData(USART2);

		if((USART2_RX_STA)==0 && data==0xAA)//֡ͷ
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			USART2_RX_STA++;
					
		}
		else if((USART2_RX_STA)==1 && (data==0xAF || data==0xFF))//Ŀ���ַ
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			USART2_RX_STA++;
			
		}
		else if((USART2_RX_STA)==2 &&  (data==0x00 || data==0xA0))	//������00ΪУ��֡A0 Ϊlog�ַ���
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
		else if((USART2_RX_STA)==(4+USART2_RX_BUF[3]))//��У��
		{
			USART2_RX_BUF[USART2_RX_STA&0x00FF]=data;
			USART2_RX_STA++;
			
		}
		else if((USART2_RX_STA)==(5+USART2_RX_BUF[3]))//����У��
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
	if(*(data+2)==0x00)//����
	{
		
	}
	else if(*(data+2)==0xA0)
	{
		sent_utf8_txt(data,len);
	}
	
}

#endif
