#include "MoveControl.h"

//data transfer
static uint8_t MoveControl_datatemp[50], OVControl_datatemp[50], func_code_u2, func_code_u3;
//fly control variable
static uint16_t distance_cm, velocity_cmps, dir_angle_0_360, spin_angle_0_360, spin_speed_dps;
//number identify
static uint8_t number_set = 3, percentage_set = 30, number_identified, percentage_identified;
//circling motion variable
static uint8_t origin_dir, cir_motion_dir, cir_motion_r_cm, cir_motion_speed_dps;
static uint16_t cir_motion_degrees;
//MoveControl Get OneByte from USART2
void MoveControl_GetOneByte(uint8_t data)
{
	static u8 _data_len_u2 = 0, _data_cnt_u2 = 0;
	static u8 rxstate_u2 = 0;

	if (rxstate_u2 == 0 && data == 0xAA)
	{
		//HEAD
		rxstate_u2 = 1;
		MoveControl_datatemp[0] = data;
	}
	else if (rxstate_u2 == 1 && (data == HW_TYPE || data == HW_ALL))
	{
		//Dest
		rxstate_u2 = 2;
		MoveControl_datatemp[1] = data;
	}
	else if (rxstate_u2 == 2)
	{
		//Func
		rxstate_u2 = 3;
		MoveControl_datatemp[2] = data;
	}
	else if (rxstate_u2 == 3 && data < 250)
	{
		//Len
		rxstate_u2 = 4;
		MoveControl_datatemp[3] = data;
		_data_len_u2 = data;
		_data_cnt_u2 = 0;
	}
	else if (rxstate_u2 == 4 && _data_len_u2 > 0)
	{
		//data
		_data_len_u2--;
		MoveControl_datatemp[4 + _data_cnt_u2++] = data;
		if (_data_len_u2 == 0)
			rxstate_u2 = 5;
	}
	else if (rxstate_u2 == 5)
	{
		//SumCheck
		rxstate_u2 = 6;
		MoveControl_datatemp[4 + _data_cnt_u2++] = data;
	}
	else if (rxstate_u2 == 6)
	{
		//add on check
		rxstate_u2 = 0;
		MoveControl_datatemp[4 + _data_cnt_u2] = data;
		//DT_data_cnt = _data_cnt_u2+5;
		//data resolution
		MoveControl_DataAnl(MoveControl_datatemp, _data_cnt_u2 + 5);
	}
	else
	{
		//error
		rxstate_u2 = 0;
	}
}

//OVControl_GetOneByte
void OVControl_GetOneByte(uint8_t data)
{
	static u8 _data_len_u3 = 0, _data_cnt_u3 = 0;
	static u8 rxstate_u3 = 0;

	if (rxstate_u3 == 0 && data == 0xAA)
	{
		//HEAD
		rxstate_u3 = 1;
		OVControl_datatemp[0] = data;
	}
	else if (rxstate_u3 == 1 && (data == HW_TYPE || data == HW_ALL))
	{
		//Dest
		rxstate_u3 = 2;
		OVControl_datatemp[1] = data;
	}
	else if (rxstate_u3 == 2)
	{
		//Func
		rxstate_u3 = 3;
		OVControl_datatemp[2] = data;
	}
	else if (rxstate_u3 == 3 && data < 250)
	{
		//Len
		rxstate_u3 = 4;
		OVControl_datatemp[3] = data;
		_data_len_u3 = data;
		_data_cnt_u3 = 0;
	}
	else if (rxstate_u3 == 4 && _data_len_u3 > 0)
	{
		//data
		_data_len_u3--;
		OVControl_datatemp[4 + _data_cnt_u3++] = data;
		if (_data_len_u3 == 0)
			rxstate_u3 = 5;
	}
	else if (rxstate_u3 == 5)
	{
		//SumCheck
		rxstate_u3 = 6;
		OVControl_datatemp[4 + _data_cnt_u3++] = data;
	}
	else if (rxstate_u3 == 6)
	{
		//add on check
		rxstate_u3 = 0;
		OVControl_datatemp[4 + _data_cnt_u3] = data;
		//DT_data_cnt = _data_cnt_u2+5;
		//data resolution
		OVControl_DataAnl(OVControl_datatemp, _data_cnt_u3 + 5);
	}
	else
	{
		//error
		rxstate_u3 = 0;
	}
}

//MoveControl data resolution
void MoveControl_DataAnl(uint8_t *data, uint8_t len)
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
	func_code_u2 = (*(data + 2));
	
	if (func_code_u2 == 0x80)
	{ 
		//fly control
		distance_cm = (*(data + 4) << 8) | (*(data + 5));
		velocity_cmps = (*(data + 6) << 8) | (*(data + 7));
		dir_angle_0_360 = (*(data + 8) << 8) | (*(data + 9));
		spin_angle_0_360 = (*(data + 10) << 8) | (*(data + 11));
		spin_speed_dps = (*(data + 12) << 8) | (*(data + 13));
	}
	else if(func_code_u2 == 0x82)
	{
		//circling motion
		origin_dir = (*(data + 4));
		cir_motion_dir = (*(data + 5));
		cir_motion_r_cm = (*(data + 6));
		cir_motion_speed_dps = (*(data + 7));
		cir_motion_dir = (*(data + 7) << 8) | (*(data + 9));
	}
}

//OVControl_DataAnl
void OVControl_DataAnl(uint8_t *data, uint8_t len)
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
	func_code_u3 = (*(data + 2));
	
	if (func_code_u3 == 0x90)
	{
		//number identify
		number_identified = (*(data + 4));
		percentage_identified = (*(data + 5));
	}
	
}

static u8 mission_f = 0;
static u8 mission_step = 0;
static u16 time_dly_cnt_ms;
u16 control_stat = 1;
//MoveControl Output
void MoveControl_Output(void)
{
	if (mission_f == 0)
	{
		switch (mission_step)
		{
			case 0:
			{
				//reset
				time_dly_cnt_ms = 0;
				mission_step++;
			}
			break;
			case 1:
			{
				//switch to program control
				mission_step += LX_Change_Mode(3);
			}
			break;
			case 2:
			{
				//unlock check
				mission_step += UNLOCK_STATE;
				//mission_step++;
			}
			break;
			case 3:
			{
				if (time_dly_cnt_ms < 2000)
				{
					time_dly_cnt_ms += 50; //ms
				}
				else
				{
					time_dly_cnt_ms = 0;
					mission_step++;
				}
			}
			break;
			case 4:
			{
				//takeoff
				mission_step += OneKey_Takeoff(80);
			}
			break;
			case 5:
			{
				if (time_dly_cnt_ms < 5000)
				{
					time_dly_cnt_ms += 50;
				}
				else
				{
					time_dly_cnt_ms = 0;
					mission_step++;
				}
			}
			break;
			case 6: //user instructions
			{
				if (UNLOCK_STATE)
				//if(1)
				{
					if (func_code_u2 == 0x80) //fly control
					{

						time_dly_cnt_ms = 0;
						if (control_stat == 1)
						{
							Horizontal_Move(distance_cm, velocity_cmps, dir_angle_0_360);
							control_stat = 0;
						}
						else
						{
							control_stat = 1;
							MoveControl_Spin(spin_angle_0_360, spin_speed_dps);
						}
					}
					else if (func_code_u2 == 0x81) //land
					{
						time_dly_cnt_ms += 50;
						if (time_dly_cnt_ms >= 3000)
						{
							OneKey_Land();
							mission_step = 2;
						}
					}
					else if (func_code_u2 == 0x90 || func_code_u3 == 0x90) //identify specific number
					{
						if (number_identified == number_set && percentage_identified >= percentage_set)
							OneKey_Land();
						mission_step = 2;
					}
				}
			}
		}
	}
}

//MoveControl Spin
uint8_t MoveControl_Spin(uint16_t spin_angle_0_360, uint16_t spin_speed_dps)
{
	if (dt.wait_ck == 0) //there is no wait ack
	{
		dt.cmd_send.CID = 0x10;
		dt.cmd_send.CMD[0] = 0x02;
		if (spin_angle_0_360 <= 180)
		{
			dt.cmd_send.CMD[1] = 0x08;
		}
		else
		{
			spin_angle_0_360 = 360 - spin_angle_0_360;
			dt.cmd_send.CMD[1] = 0x07;
		}
		dt.cmd_send.CMD[2] = BYTE0(spin_angle_0_360);
		dt.cmd_send.CMD[3] = BYTE1(spin_angle_0_360);
		dt.cmd_send.CMD[4] = BYTE0(spin_speed_dps);
		dt.cmd_send.CMD[5] = BYTE1(spin_speed_dps);
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//Circling motion
//The origin is perpendicular to the head 
uint8_t Circling_motion(uint8_t origin_dir, uint8_t cir_motion_dir, uint8_t cir_motion_r_cm, uint8_t cir_motion_speed_dps, uint16_t cir_motion_degrees)
{
	uint8_t move_distance_cm = cir_motion_degrees * RAD_PER_DEG * cir_motion_r_cm;// l = a*r
	uint8_t time_s = cir_motion_degrees / cir_motion_speed_dps;// t = a/w
	uint8_t move_velocity_cms = move_distance_cm / time_s;// v = l/t
	uint8_t spin_speed = cir_motion_speed_dps;
	uint16_t move_dir = 0;
	uint8_t spin_dir = 0;
	
	if(origin_dir == 0 && cir_motion_dir == 0)
	{
		move_dir = 180;
		spin_dir = 1;
	}
	else if(origin_dir == 0 && cir_motion_dir == 1)
	{
		move_dir = 0;
		spin_dir = 0;
	}
	else if(origin_dir == 1 && cir_motion_dir == 0)
	{
		move_dir = 0;
		spin_dir = 1;
	}
	else if(origin_dir == 1 && cir_motion_dir == 1)
	{
		move_dir = 180;
		spin_dir = 0;
	}

	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X03;
		dt.cmd_send.CMD[2] = 0;
		dt.cmd_send.CMD[3] = BYTE1(move_distance_cm);
		dt.cmd_send.CMD[4] = 0;
		dt.cmd_send.CMD[5] = BYTE1(move_velocity_cms);
		dt.cmd_send.CMD[6] = BYTE0(move_dir);
		dt.cmd_send.CMD[7] = BYTE1(move_dir);
		CMD_Send(0xff, &dt.cmd_send);

		dt.cmd_send.CID = 0x10;
		dt.cmd_send.CMD[0] = 0x02;
		if (spin_dir)
		{
			dt.cmd_send.CMD[1] = 0x08;
		}
		else
		{
			dt.cmd_send.CMD[1] = 0x07;
		}
		dt.cmd_send.CMD[2] = BYTE0(cir_motion_degrees);
		dt.cmd_send.CMD[3] = BYTE1(cir_motion_degrees);
		dt.cmd_send.CMD[4] = 0;
		dt.cmd_send.CMD[5] = BYTE1(spin_speed);
		CMD_Send(0xff, &dt.cmd_send);

		return 1;
	}
	else
	{
		return 0;
	}
}


void MoveTo_LandingPad(void){
	
	uint16_t distance=0;
	uint16_t velocity=0;
	uint16_t direction=0;
	uint16_t P=1;
	uint8_t Landing_STA=0; 
	//First STA=1-3//back to first line
	//Second STA=4-6//move to 
	//Third STA=7-9
	
	
	uint8_t databuffer[]={0xAA,0xFF,0xED,0x09,0x10,0x02,0x03,0x01,0x02,0x03,0x04,0x05,0x06,0x00,0x00};//len=15
	if(state_is_land!=3){
		return ;
	}
	
	
	if(Landing_STA==0)
	{
		Landing_STA=1;
	}
	else if(Landing_STA<4&& Landing_STA>0)
	{
		distance=abs(t265_y_position_control);
		velocity=P*distance;
		if(velocity>20) velocity=20;
		databuffer[8]=distance>>8;
		databuffer[7]=distance-(databuffer[8]<<8);
		databuffer[10]=velocity>>8;
		databuffer[9]=velocity-(databuffer[8]<<8);
		

		if(t265_y_position_control>0){
			direction=90;
			databuffer[12]=direction>>8;
			databuffer[11]=direction-(databuffer[8]<<8);
		}
		else if(t265_y_position_control<0){
			direction=270;
			databuffer[12]=direction>>8;
			databuffer[11]=direction-(databuffer[8]<<8);
		}
		
		sent_MoveData_ToANO(databuffer,15);
		
		if(t265_y_position_control<1 &&t265_y_position_control>1){
			Landing_STA++;
		}
		
	}
	else if(Landing_STA<7&& Landing_STA>3)
	{
		distance=abs(t265_x_position_control);
		velocity=P*distance;
		if(velocity>20) velocity=20;
		databuffer[8]=distance>>8;
		databuffer[7]=distance-(databuffer[8]<<8);
		databuffer[10]=velocity>>8;
		databuffer[9]=velocity-(databuffer[8]<<8);
		

		if(t265_y_position_control>0){
			direction=180;
			databuffer[12]=direction>>8;
			databuffer[11]=direction-(databuffer[8]<<8);
		}
		else if(t265_y_position_control<0){
			direction=0;
			databuffer[12]=direction>>8;
			databuffer[11]=direction-(databuffer[8]<<8);
		}
		
		sent_MoveData_ToANO(databuffer,15);
		
		if(t265_y_position_control<1 &&t265_y_position_control>1){
			Landing_STA++;
		}
		
	}
	else if(Landing_STA<10&& Landing_STA>6)
	{
		distance=abs(barcode_data*10-t265_x_position_control);
		velocity=P*distance;
		if(velocity>20) velocity=20;
		databuffer[8]=distance>>8;
		databuffer[7]=distance-(databuffer[8]<<8);
		databuffer[10]=velocity>>8;
		databuffer[9]=velocity-(databuffer[8]<<8);
		

		if(t265_y_position_control>(barcode_data*10)){
			direction=180;
			databuffer[12]=direction>>8;
			databuffer[11]=direction-(databuffer[8]<<8);
		}
		else if(t265_y_position_control<(barcode_data*10)){
			direction=0;
			databuffer[12]=direction>>8;
			databuffer[11]=direction-(databuffer[8]<<8);
		}
		
		sent_MoveData_ToANO(databuffer,15);
		
		if(t265_y_position_control+1>(barcode_data*10) && t265_y_position_control-1<(barcode_data*10)){
			Landing_STA++;
		}
		
	}else{
		MainBoard_OneKeyLand();
	}
	
}




void sent_MoveData_ToANO(uint8_t* data,uint8_t len){
	
	
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














\






