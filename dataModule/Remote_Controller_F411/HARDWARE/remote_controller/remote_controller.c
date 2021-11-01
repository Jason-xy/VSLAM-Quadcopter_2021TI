#include "remote_controller.h"



void sent_CMD_ACC_calibration(void){
	char data[]={0xAA,0xFF,0xE0,0x03,0x01,0x00,0x01,0x8E,0x10};
	int len=9;
	int t=0;
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, data[t]);         //向串口1发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	
}
void sent_CMD_GYRO_calibration(void)
{
	char data[]={0xAA,0xFF,0xE0,0x03,0x01,0x00,0x02,0x8F,0x11};
	int len=9;
	int t=0;
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, data[t]);         //向串口1发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}
void sent_CMD_RH_calibration(void){//Rapid horizontal calibration
	char data[]={0xAA,0xFF,0xE0,0x03,0x01,0x00,0x03,0x90,0x12};
	int len=9;
	int t=0;
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, data[t]);         //向串口1发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}
void sent_CMD_MAG_calibration(void){
	char data[]={0xAA,0xFF,0xE0,0x03,0x01,0x00,0x04,0x91,0x13};
	int len=9;
	int t=0;
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, data[t]);         //向串口1发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}
void sent_CMD_SixSides_calibration(void){
	char data[]={0xAA,0xFF,0xE0,0x03,0x01,0x00,0x05,0x92,0x14};
	int len=9;
	int t=0;
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, data[t]);         //向串口1发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}
void sent_CMD_unlock(void){
	char data[]={0xAA,0xFF,0xE0,0x03,0x10,0x00,0x01,0x9D,0x3D};
	int len=9;
	int t=0;
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, data[t]);         //向串口1发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}
void sent_CMD_Estop(void){
	char data[]={0xAA,0xFF,0xE0,0x03,0x10,0x00,0x02,0x9E,0x3E};
	int len=9;
	int t=0;
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, data[t]);         //向串口1发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}


void sent_utf8_txt(u8* str,u8 len){

	int t=0;
	
	printf("t0.txt=t1.txt ");
	for(t=0;t<3;t++)
	{
		USART_SendData(USART1, 0xFF);         //向串口1发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}
	
	
	printf("t1.txt=t2.txt ");
	for(t=0;t<3;t++)
	{
		USART_SendData(USART1, 0xFF);         //向串口1发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}
	
	printf("t2.txt=t3.txt ");
	for(t=0;t<3;t++)
	{
		USART_SendData(USART1, 0xFF);         //向串口1发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}
	
	
	str[4+str[3]]='\0';
	printf(	"t3.txt =\" %s  \" ",(str+5));
	for(t=0;t<3;t++)
	{
		USART_SendData(USART1, 0xFF);         //向串口1发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}
	
}


