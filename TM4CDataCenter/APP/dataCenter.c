#include "dataCenter.h"

//data buffer
static uint8_t MainBoard_datatemp[100];
static uint8_t OpenMV_datatemp[100];
static uint8_t K210_datatemp[100];
static uint8_t Jetson_datatemp[100];
static uint8_t TOF_datatemp[100];
static uint8_t Screen_datatemp[100];

//OpenMV
static uint8_t minL;
static uint8_t maxL;
static uint8_t minA;
static uint8_t maxA;
static uint8_t minB;
static uint8_t maxB;

//Jetson

//





//GetOneByte Function
//MainBoard:U2
void MainBoard_GetOneByte(uint8_t data)
{
	static u8 _data_len_u2 = 0, _data_cnt_u2 = 0;
	static u8 rxstate_u2 = 0;

	if (rxstate_u2 == 0 && data == 0xAA)
	{
		//HEAD
		rxstate_u2 = 1;
		MainBoard_datatemp[0] = data;
    }
	else if (rxstate_u2 == 1 && (data == HW_TYPE || data == HW_ALL))
	{
		//Dest
		rxstate_u2 = 2;
		MainBoard_datatemp[1] = data;
	}
	else if (rxstate_u2 == 2)
	{
		//Func
		rxstate_u2 = 3;
		MainBoard_datatemp[2] = data;
	}
	else if (rxstate_u2== 3 && data < 250)
	{
		//Len
		rxstate_u2 = 4;
		MainBoard_datatemp[3] = data;
		_data_len_u2 = data;
		_data_cnt_u2 = 0;
	}
	else if (rxstate_u2 == 4 && _data_len_u2 > 0)
	{
		//data
		_data_len_u2--;
		MainBoard_datatemp[4 + _data_cnt_u2++] = data;
		if (_data_len_u2 == 0)
			rxstate_u2 = 5;
	}
	else if (rxstate_u2 == 5)
	{
		//SumCheck
		rxstate_u2 = 6;
		MainBoard_datatemp[4 + _data_cnt_u2++] = data;
	}
	else if (rxstate_u2 == 6)
	{
		//add on check
		rxstate_u2 = 0;
		MainBoard_datatemp[4 + _data_cnt_u2] = data;
		//DT_data_cnt = _data_cnt_u2+5;
		//data resolution
		MainBoard_DataAnl(MainBoard_datatemp, _data_cnt_u2 + 5);
	}
	else
	{
		//error
		rxstate_u2 = 0;
	}
}

//OpenMV:U3
void OpenMV_GetOneByte(uint8_t data)
{
	static u8 _data_len_u3 = 0, _data_cnt_u3 = 0;
	static u8 rxstate_u3 = 0;

	if (rxstate_u3 == 0 && data == 0xAA)
	{
		//HEAD
		rxstate_u3 = 1;
		OpenMV_datatemp[0] = data;
    }
	else if (rxstate_u3 == 1 && (data == HW_TYPE || data == HW_ALL))
	{
		//Dest
		rxstate_u3 = 2;
		OpenMV_datatemp[1] = data;
	}
	else if (rxstate_u3 == 2)
	{
		//Func
		rxstate_u3 = 3;
		OpenMV_datatemp[2] = data;
	}
	else if (rxstate_u3== 3 && data < 250)
	{
		//Len
		rxstate_u3 = 4;
		OpenMV_datatemp[3] = data;
		_data_len_u3 = data;
		_data_cnt_u3 = 0;
		if (_data_len_u3 == 0)
			rxstate_u3 = 5;
	}
	else if (rxstate_u3 == 4 && _data_len_u3 >= 0)
	{
		//data
		_data_len_u3--;
		OpenMV_datatemp[4 + _data_cnt_u3++] = data;
		if (_data_len_u3 == 0)
			rxstate_u3 = 5;
	}
	else if (rxstate_u3 == 5)
	{
		//SumCheck
		rxstate_u3 = 6;
		OpenMV_datatemp[4 + _data_cnt_u3++] = data;
	}
	else if (rxstate_u3 == 6)
	{
		//add on check
		rxstate_u3 = 0;
		OpenMV_datatemp[4 + _data_cnt_u3] = data;
		//DT_data_cnt = _data_cnt_u2+5;
		//data resolution
		OpenMV_DataAnl(OpenMV_datatemp, _data_cnt_u3 + 5);
	}
	else
	{
		//error
		rxstate_u3 = 0;
	}
}

//K210:U4
void K210_GetOneByte(uint8_t data)
{
	static u8 _data_len_u4 = 0, _data_cnt_u4 = 0;
	static u8 rxstate_u4 = 0;

	if (rxstate_u4 == 0 && data == 0xAA)
	{
		//HEAD
		rxstate_u4 = 1;
		K210_datatemp[0] = data;
    }
	else if (rxstate_u4 == 1 && (data == HW_TYPE || data == HW_ALL))
	{
		//Dest
		rxstate_u4 = 2;
		K210_datatemp[1] = data;
	}
	else if (rxstate_u4 == 2)
	{
		//Func
		rxstate_u4 = 3;
		K210_datatemp[2] = data;
	}
	else if (rxstate_u4== 3 && data < 250)
	{
		//Len
		rxstate_u4 = 4;
		K210_datatemp[3] = data;
		_data_len_u4 = data;
		_data_cnt_u4 = 0;
	}
	else if (rxstate_u4 == 4 && _data_len_u4 > 0)
	{
		//data
		_data_len_u4--;
		K210_datatemp[4 + _data_cnt_u4++] = data;
		if (_data_len_u4 == 0)
			rxstate_u4 = 5;
	}
	else if (rxstate_u4 == 5)
	{
		//SumCheck
		rxstate_u4 = 6;
		K210_datatemp[4 + _data_cnt_u4++] = data;
	}
	else if (rxstate_u4 == 6)
	{
		//add on check
		rxstate_u4 = 0;
		K210_datatemp[4 + _data_cnt_u4] = data;
		//DT_data_cnt = _data_cnt_u2+5;
		//data resolution
		K210_DataAnl(K210_datatemp, _data_cnt_u4 + 5);
	}
	else
	{
		//error
		rxstate_u4 = 0;
	}
}

//Jetson:U5
void Jetson_GetOneByte(uint8_t data)
 {
	static u8 _data_len_u5 = 0, _data_cnt_u5 = 0;
	static u8 rxstate_u5 = 0;

	if (rxstate_u5 == 0 && data == 0xAA)
	{
		//HEAD
		rxstate_u5 = 1;
		Jetson_datatemp[0] = data;
    }
	else if (rxstate_u5 == 1 && (data == HW_TYPE || data == HW_ALL))
	{
		//Dest
		rxstate_u5 = 2;
		Jetson_datatemp[1] = data;
	}
	else if (rxstate_u5 == 2)
	{
		//Func
		rxstate_u5 = 3;
		Jetson_datatemp[2] = data;
	}
	else if (rxstate_u5== 3 && data < 250)
	{
		//Len
		rxstate_u5 = 4;
		Jetson_datatemp[3] = data;
		_data_len_u5 = data;
		_data_cnt_u5 = 0;
	}
	else if (rxstate_u5 == 4 && _data_len_u5 > 0)
	{
		//data
		_data_len_u5--;
		Jetson_datatemp[4 + _data_cnt_u5++] = data;
		if (_data_len_u5 == 0)
			rxstate_u5 = 5;
	}
	else if (rxstate_u5 == 5)
	{
		//SumCheck
		rxstate_u5 = 6;
		Jetson_datatemp[4 + _data_cnt_u5++] = data;
	}
	else if (rxstate_u5 == 6)
	{
		//add on check
		rxstate_u5 = 0;
		Jetson_datatemp[4 + _data_cnt_u5] = data;
		//DT_data_cnt = _data_cnt_u2+5;
		//data resolution
		Jetson_DataAnl(Jetson_datatemp, _data_cnt_u5 + 5);
	}
	else
	{
		//error
		rxstate_u5 = 0;
	}
}

//TOF:U6
void TOF_GetOneByte(uint8_t data)
{

}

//Screen:U7
void Screen_GetOneByte(uint8_t data)
{
	static u8 _data_len_u7 = 0, _data_cnt_u7 = 0;
	static u8 rxstate_u7 = 0;
	static u8 check_sum1_len=0;
	static u8 check_sum2_len=0;

	if (rxstate_u7 == 0 && data == 0xAA)
	{
		//HEAD
		rxstate_u7 = 1;
		Screen_datatemp[0] = data;
    }
	else if (rxstate_u7 == 1 && (data == HW_TYPE || data == HW_ALL))
	{
		//Dest
		rxstate_u7 = 2;
		Screen_datatemp[1] = data;
	}
	else if (rxstate_u7 == 2)
	{
		//Func
		rxstate_u7 = 3;
		Screen_datatemp[2] = data;
	}
	else if (rxstate_u7== 3 && data < 250)
	{
		//Len
		rxstate_u7 = 4;
		Screen_datatemp[3] = data;
		_data_len_u7 = data;
		_data_cnt_u7 = 0;
		if (_data_len_u7 == 0)
			rxstate_u7 = 5;
	}
	else if (rxstate_u7 == 4 && _data_len_u7 >= 0)
	{	
		//data
		_data_len_u7--;
		Screen_datatemp[4 + _data_cnt_u7++] = data;
		if (_data_len_u7 == 0)
			rxstate_u7 = 5;
	}
	else if (rxstate_u7 == 5)
	{
		
		//SumCheck
		Screen_datatemp[4 + _data_cnt_u7++] = data;
		check_sum1_len++;
		if (check_sum1_len == 4){
			check_sum1_len =0;
			rxstate_u7 = 6;
		}
		//SumCheck
//		rxstate_u7 = 6;
//		Screen_datatemp[4 + _data_cnt_u7++] = data;
	}
	else if (rxstate_u7 == 6)
	{
		//SumCheck
		Screen_datatemp[4 + _data_cnt_u7++] = data;
		check_sum2_len++;
		if (check_sum2_len == 4){
			check_sum2_len =0;
			rxstate_u7 = 0;
			Screen_DataAnl(Screen_datatemp, _data_cnt_u7 + 4);
		}		
		//add on check
//		rxstate_u7 = 0;
//		Screen_datatemp[4 + _data_cnt_u7] = data;
//		//DT_data_cnt = _data_cnt_u2+5;
//		//data resolution
//		Screen_DataAnl(Screen_datatemp, _data_cnt_u7 + 5);
	}
	else
	{
		//error
		rxstate_u7 = 0;
	}
}

//Data Analysis Function
void MainBoard_DataAnl(uint8_t *data, uint8_t len)
{
    //data check
	uint8_t check_sum1 = 0, check_sum2 = 0;
	if (*(data + 3) != (len - 6)) //check the length of the data
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		check_sum1 += *(data + i); //Sum check
		check_sum2 += check_sum1;  //add on check
	}
	if ((check_sum1 != *(data + len - 2)) || (check_sum2 != *(data + len - 1))) //compare the checksum
		return;
	/*================================================================================*/
	//Get data
}




void OpenMV_DataAnl(uint8_t *data, uint8_t len)
{
    //data check
	uint8_t check_sum1 = 0, check_sum2 = 0;
	if (*(data + 3) != (len - 6)) //check the length of the data
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		check_sum1 += *(data + i); //Sum check
		check_sum2 += check_sum1;  //add on check
	}
	if ((check_sum1 != *(data + len - 2)) || (check_sum2 != *(data + len - 1))) //compare the checksum
		return;
	/*================================================================================*/
	//Get data
	if(*(data+1)!=0x62)
		return;
	switch(*(data+2))
	{
		case 0xa4://OpenMV sent LAB
			minL=*(data+4);
			maxL=*(data+5);
			minA=*(data+6);
			maxA=*(data+7);
			minB=*(data+8);
			maxB=*(data+9);
			Screen_BackData();
			break;
		default:
			break;
	}
}

void K210_DataAnl(uint8_t *data, uint8_t len)
{
    //data check
	uint8_t check_sum1 = 0, check_sum2 = 0;
	if (*(data + 3) != (len - 6)) //check the length of the data
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		check_sum1 += *(data + i); //Sum check
		check_sum2 += check_sum1;  //add on check
	}
	if ((check_sum1 != *(data + len - 2)) || (check_sum2 != *(data + len - 1))) //compare the checksum
		return;
	/*================================================================================*/
	//Get data

}

void Jetson_DataAnl(uint8_t *data, uint8_t len)
{
    //data check
	uint8_t check_sum1 = 0, check_sum2 = 0;
	if (*(data + 3) != (len - 6)) //check the length of the data
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		check_sum1 += *(data + i); //Sum check
		check_sum2 += check_sum1;  //add on check
	}
	if ((check_sum1 != *(data + len - 2)) || (check_sum2 != *(data + len - 1))) //compare the checksum
		return;
	/*================================================================================*/
	//Get data
	


}

void TOF_DataAnl(uint8_t *data, uint8_t len)
{

}

void Screen_DataAnl(uint8_t *data, uint8_t len)
{
    //data check
	//Screen Data is little endding so check_sum in data is -2 and -4
	uint8_t check_sum1 = 0, check_sum2 = 0;
	if (*(data + 3) != (len - 12)) //check the length of the data
		return;
	for (uint8_t i = 0; i < len - 8; i++)
	{
		check_sum1 += *(data + i); //Sum check
		check_sum2 += check_sum1;  //add on check
		if(i>3)i=i+3;
	}
	if ((check_sum1 != *(data + len - 8)) || (check_sum2 != *(data + len - 4))) //compare the checksum
		return;
	/*================================================================================*/
	//Get data
	if(*(data+1)!=0x62)
		return;
	switch(*(data+2))
	{
		case 0xa0://Screen sent LAB
			minL=*(data+4);
			maxL=*(data+8);
			minA=*(data+12);
			maxA=*(data+16);
			minB=*(data+20);
			maxB=*(data+24);
			OpenMV_SetLAB();
			break;
		case 0xa1:
			OpenMV_RequestLAB();
			break;
		default:
			break;
	}
	
	
}


void OpenMV_RequestLAB(void)
{
	//0xa3
	uint8_t OpenMV_Buf[]={0xaa,0x63,0xa3,0x00,0xB0,0x17};
	DrvUart3SendBuf(OpenMV_Buf, (u8)6);
		
}
void OpenMV_SetLAB(void)
{
	//0xa2
	uint8_t OpenMV_Buf[]={0xaa,0x63,0xa2,0x06,0x01,0x02,0x03,0x04,0x05,0x06,0x01,0x02};
	uint8_t check_sum1=0,check_sum2=0;
	OpenMV_Buf[4]=minL;
	OpenMV_Buf[5]=maxL;
	OpenMV_Buf[6]=minA;
	OpenMV_Buf[7]=maxA;
	OpenMV_Buf[8]=minB;
	OpenMV_Buf[9]=maxB;
	for(uint8_t i=0;i<10;i++){
		check_sum1+=OpenMV_Buf[i];
		check_sum2+=check_sum1;		
	}
	OpenMV_Buf[10]=check_sum1;
	OpenMV_Buf[11]=check_sum2;
	DrvUart3SendBuf(OpenMV_Buf, (u8)12);
}


void Screen_BackData(void)
{
	static uint8_t Screen_Buf[]={0xaa,0x67,0xa7,0x06,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00};
	uint8_t check_sum1=0,check_sum2=0;
	Screen_Buf[4]=minL;
	Screen_Buf[8]=maxL;
	Screen_Buf[12]=minA;
	Screen_Buf[16]=maxA;
	Screen_Buf[20]=minB;
	Screen_Buf[24]=maxB;
	for(uint8_t i=0;i<28;i++){
		check_sum1+=Screen_Buf[i];
		check_sum2+=check_sum1;
		if(i>3)i=i+3;
	}
	Screen_Buf[28]=check_sum1;
	Screen_Buf[32]=check_sum2;
	DrvUart7SendBuf(Screen_Buf, (u8)40);
}

