#include "dataCenter.h"

//rx data buffer
static uint8_t MainBoard_datatemp[100];
static uint8_t OpenMV_datatemp[100];
static uint8_t K210_datatemp[100];
static uint8_t Jetson_datatemp[100];
static uint8_t BarcodeScanner_datatemp[100];
static uint8_t Screen_datatemp[100];
//tx data buffer
static uint8_t Jetson_datatemp_tx[100];

//OpenMV
static uint8_t minL;
static uint8_t maxL;
static uint8_t minA;
static uint8_t maxA;
static uint8_t minB;
static uint8_t maxB;

//Jetson
static int16_t t265_x_velocity_cmps;
static int16_t t265_y_velocity_cmps;
static int16_t t265_z_velocity_cmps;
static int16_t t265_x_position;
static int16_t t265_y_position;
static int16_t t265_z_position;

int16_t t265_x_position_control;
int16_t t265_y_position_control;

//is barScanner have data if got change the mode to 1
uint8_t state_is_land=0;// only scaned 1 only allowed 2     scaned and allowed 3
uint8_t barcode_data=0;





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

//BarcodeScanner:U6
void BarcodeScanner_GetOneByte(uint8_t data)
{
	//BarcodeScanner_datatemp
	static uint8_t BarcodeScanner_STA=0;
	//recieved   \r sta=1 \n
	if(BarcodeScanner_STA==0  )
	{
		if(data=='\r'){
			return;
	}
		BarcodeScanner_datatemp[BarcodeScanner_STA]=data;
		BarcodeScanner_STA++;
	}
	else if(BarcodeScanner_datatemp[BarcodeScanner_STA-1]!='\r')
	{
		BarcodeScanner_datatemp[BarcodeScanner_STA]=data;
		BarcodeScanner_STA++;	
	}
	else if(BarcodeScanner_datatemp[BarcodeScanner_STA-1]=='\r')
	{
		if(data=='\n')
		{
			BarcodeScanner_datatemp[BarcodeScanner_STA]=data;
			BarcodeScanner_STA++;
			BarcodeScanner_DataAnl(BarcodeScanner_datatemp,BarcodeScanner_STA);
		}
		
		BarcodeScanner_STA=0;
	}
	
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

int8_t blink_state = 0;
int8_t preblink_state = 0;
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
	switch(*(data+2)){
		case 0x80:
			*(data+1)=0x61;
			sent_Control_ToANO(data,len);//Modifying the Device Number£¬count check_sum
			break;
		case 0x91:
			//DrvUart2SendBuf(data, (u8)18);
			t265_x_position = (*(data + 5) << 8) | (*(data + 4));
			t265_y_position = (*(data + 7) << 8) | (*(data + 6));
			t265_z_position = (*(data + 9) << 8) | (*(data + 8));
			t265_x_velocity_cmps = (*(data + 11) << 8) | (*(data + 10));
			t265_y_velocity_cmps = (*(data + 13) << 8) | (*(data + 12));
			t265_z_velocity_cmps = (*(data + 15) << 8) | (*(data + 14));
			send_t265_data();//sent to mainboard
			break;
		case 0x92:
			if(preblink_state != (*(data+4)))
				blink_state = 1;
			preblink_state = (*(data+4));
			break;
		default:
			break;
	}
}

void BarcodeScanner_DataAnl(uint8_t *data, uint8_t len)
{
	int i=0;
	if(*(data+len-1)!='\n'||*(data+len-2)!='\r')
	{
		return;
	}
	
	if(state_is_land==1||state_is_land==0){
		
		state_is_land=1;
	}
	else if(state_is_land==2){
		state_is_land=3;
	}
	for(i=0;i<len-2;i++){
		barcode_data*=10;
		barcode_data=*(data+i)-'0';
	}
	Barcode_Blink();
	
}

void Screen_DataAnl(uint8_t *data, uint8_t len)
{
    //data check
	//Screen Data is little endding so check_sum in data is -2 and -4y
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
		case 0xa1://Screen Wants LAB
			OpenMV_RequestLAB();
			break;
		case 0xa8://Tell 61 six sides
			MainBoard_BeginSixSidesCalibration();
			break;
		case 0xa9://tell 61 compass
			MainBoard_BeginCompassCalibration();
			break;
		case 0xaa://tell 62 level
			MainBoard_BeginLevelCalibration();
			break;
		case 0xab://tell 61 unlock
			MainBoard_Unlock();
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

void MainBoard_BeginSixSidesCalibration(void)
{
	uint8_t MainBoard_Buf[]={0xaa,0xff,0xe0,0x03,0x01,0x00,0x05,0x92,0x14};
	DrvUart2SendBuf(MainBoard_Buf, (u8)10);
}
void MainBoard_BeginCompassCalibration(void)
{
	uint8_t MainBoard_Buf[]={0xaa,0xff,0xe0,0x03,0x01,0x00,0x04,0x91,0x13};
	DrvUart2SendBuf(MainBoard_Buf, (u8)10);
}
void MainBoard_BeginLevelCalibration(void)
{
	uint8_t MainBoard_Buf[]={0xaa,0xff,0xe0,0x03,0x01,0x00,0x03,0x90,0x12};
	DrvUart2SendBuf(MainBoard_Buf, (u8)10);
}
void MainBoard_Unlock(void)
{
	uint8_t MainBoard_Buf[]={0xaa,0xff,0xe0,0x03,0x10,0x00,0x01,0x9d,0x3d};
	DrvUart2SendBuf(MainBoard_Buf, (u8)10);
}
void MainBoard_OneKeyLand(void){
	uint8_t MainBoard_Buf[]={0xaa,0xff,0xe0,0x03,0x10,0x00,0x06,0xa2,0x42};
	DrvUart2SendBuf(MainBoard_Buf, (u8)10);	
}
//t265 data
void send_t265_data(void)
{
	memset(Jetson_datatemp_tx, 0, 100);
	Jetson_datatemp_tx[0] = 0xAA;
	Jetson_datatemp_tx[1] = 0x61;
	Jetson_datatemp_tx[2] = 0x91;
	Jetson_datatemp_tx[3] = 0x0C;
	
	//x_position_cm
  Jetson_datatemp_tx[5] = t265_x_position >> 8;
  Jetson_datatemp_tx[4] = t265_x_position - (Jetson_datatemp_tx[5] << 8);
  //y_position_cm
  Jetson_datatemp_tx[7] = t265_y_position >> 8;
  Jetson_datatemp_tx[6] = t265_y_position - (Jetson_datatemp_tx[7] << 8);
  //z_position_cm
  Jetson_datatemp_tx[9] = t265_z_position >> 8;
  Jetson_datatemp_tx[8] = t265_z_position - (Jetson_datatemp_tx[9] << 8);
  //x_velocity_cms
  Jetson_datatemp_tx[11] = t265_x_velocity_cmps >> 8;
  Jetson_datatemp_tx[10] = t265_x_velocity_cmps - (Jetson_datatemp_tx[11] << 8);
  //y_velocity_cms
  Jetson_datatemp_tx[13] = t265_y_velocity_cmps >> 8;
  Jetson_datatemp_tx[12] = t265_y_velocity_cmps - (Jetson_datatemp_tx[13] << 8);
  //z_velocity_cms
  Jetson_datatemp_tx[15] = t265_z_velocity_cmps >> 8;
  Jetson_datatemp_tx[14] = t265_z_velocity_cmps - (Jetson_datatemp_tx[15] << 8);
	
	int sumcheck = 0, add_on_check = 0;
	for(int i = 0; i<= 15; i++)
  {
      sumcheck += Jetson_datatemp_tx[i];
      add_on_check += sumcheck;
  }
  sumcheck %= 256;
  add_on_check %= 256;

  Jetson_datatemp_tx[16] = sumcheck;
  Jetson_datatemp_tx[17] = add_on_check;
	
	DrvUart2SendBuf(Jetson_datatemp_tx, (u8)18);
	
}


void sent_Control_ToANO(uint8_t *data, uint8_t len){
	    //data check
	uint8_t check_sum1 = 0, check_sum2 = 0;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		check_sum1 += *(data + i); //Sum check
		check_sum2 += check_sum1;  //add on check
	}
	*(data + len - 2)=check_sum1;
	*(data + len - 1)=check_sum2;
	DrvUart2SendBuf(data,len);
}


void Barcode_Blink_Init(void){
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
 
    ROM_GPIOPinTypeGPIOOutput(GPIOA_BASE, GPIO_PIN_5);
}
void Barcode_Blink(void){
	int i=0;
	for(i=0;i<barcode_data;i++){
		GPIOPinWrite(GPIOA_BASE, GPIO_PIN_5, 0xFF);
		MyDelayMs(300);
		GPIOPinWrite(GPIOA_BASE, GPIO_PIN_5, 0);
		MyDelayMs(300);
	}
	
	MyDelayMs(3000);
}

