#include "dataCenter.h"

//data buffer
static uint8_t MainBoard_datatemp[100];
static uint8_t OpenMV_datatemp[100];
static uint8_t K210_datatemp[100];
static uint8_t Jetson_datatemp[100];
static uint8_t TOF_datatemp[100];
static uint8_t Screen_datatemp[100];

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
	}
	else if (rxstate_u3 == 4 && _data_len_u3 > 0)
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