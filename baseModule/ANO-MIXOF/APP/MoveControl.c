#include "MoveControl.h"

//data transfer
static uint8_t MoveControl_datatemp[50], func_code_u2;
//fly control variable
static uint16_t distance_cm, velocity_cmps, dir_angle_0_360, spin_angle_0_360, spin_speed_dps;
//circling motion variable
static uint8_t origin_dir, cir_motion_dir, cir_motion_r_cm, cir_motion_speed_dps;
static uint16_t cir_motion_degrees;
//V-SLAM
int16_t t265_x_velocity_cmps = 0, t265_y_velocity_cmps = 0, t265_z_velocity_cmps = 0;
int16_t t265_x_position = 0, t265_y_position = 0, t265_z_position = 0;
uint8_t t265_usart_update_cnt = 0;
int16_t t265_x_velocity_cmps_window[WINDOW_SIZE] = {0}, t265_y_velocity_cmps_window[WINDOW_SIZE] = {0}, t265_z_velocity_cmps_window[WINDOW_SIZE] = {0};
uint8_t t265_x_wp = 0, t265_y_wp = 0, t265_z_wp = 0;



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
		distance_cm = (*(data + 5) << 8) | (*(data + 4));
		velocity_cmps = (*(data + 7) << 8) | (*(data + 6));
		dir_angle_0_360 = (*(data + 9) << 8) | (*(data + 8));
		spin_angle_0_360 = (*(data + 11) << 8) | (*(data + 10));
		spin_speed_dps = (*(data + 13) << 8) | (*(data + 12));
	}
	else if(func_code_u2 == 0x82)
	{
		//circling motion
		origin_dir = (*(data + 4));
		cir_motion_dir = (*(data + 5));
		cir_motion_r_cm = (*(data + 6));
		cir_motion_speed_dps = (*(data + 7));
		cir_motion_dir = (*(data + 9) << 8) | (*(data + 8));
	}
	else if(func_code_u2 == 0x91)
	{
		//V-SLAM
		t265_x_position = (*(data + 5) << 8) | (*(data + 4));
		t265_y_position = (*(data + 7) << 8) | (*(data + 6));
		t265_z_position = (*(data + 9) << 8) | (*(data + 8));
		
		//window_smooth
		t265_x_velocity_cmps_window[t265_x_wp++ % WINDOW_SIZE] = (*(data + 11) << 8) | (*(data + 10));
		t265_y_velocity_cmps_window[t265_y_wp++ % WINDOW_SIZE] = (*(data + 13) << 8) | (*(data + 12));
		t265_z_velocity_cmps_window[t265_z_wp++ % WINDOW_SIZE] = (*(data + 15) << 8) | (*(data + 14));
		if(t265_x_wp >= WINDOW_SIZE)t265_x_wp = 0;
		if(t265_y_wp >= WINDOW_SIZE)t265_y_wp = 0;
		if(t265_z_wp >= WINDOW_SIZE)t265_z_wp = 0;
		
		t265_x_velocity_cmps = window_avg(t265_x_velocity_cmps_window, WINDOW_SIZE);
		t265_y_velocity_cmps = window_avg(t265_y_velocity_cmps_window, WINDOW_SIZE);
		t265_z_velocity_cmps = window_avg(t265_z_velocity_cmps_window, WINDOW_SIZE);
		t265_usart_update_cnt++;
	}
	else if(func_code_u2 == 0xe0)
	{
		if(*(data + 4) == 0x01 && *(data + 6) == 0x03)
		{
			Quick_Calibration();
		}
		else if(*(data + 4) == 0x01 && *(data + 6) == 0x04)
		{
			Mag_Calibration();
		}
		else if(*(data + 4) == 0x01 && *(data + 6) == 0x05)
		{
			SixAxis_Calibration();
		}
		else if(*(data + 4) == 0x10 && *(data + 6) == 0x01)
		{
			FC_Unlock();
		}
	}
}

int16_t window_avg(int16_t* arr, uint8_t len)
{
	int16_t sum = 0;
	for(uint8_t i = 0; i < len; i++)
	{
		sum += arr[i];
	}
	return sum / len;
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
				Quick_Calibration();
				MyDelayMs(10000);
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
				//FC_Unlock();
				MyDelayMs(500);
				mission_step += UNLOCK_STATE;
				//mission_step++;
			}
			break;
			case 3:
			{
				if (time_dly_cnt_ms < 5000)
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
				//mission_step += OneKey_Takeoff(30);
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
			case 6:
			{
				 //heigt control
				if(t265_z_position != 256)
					heightcontrol(80, t265_z_position);
				t265_z_position = 256;
			}
			break;
			case 7: //user instructions
			{
				if (!UNLOCK_STATE)//暂时停用
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
	uint8_t move_distance_cm = (uint8_t)(cir_motion_degrees * RAD_PER_DEG * cir_motion_r_cm);// l = a*r
	uint8_t time_s = cir_motion_degrees / cir_motion_speed_dps;// t = a/w
	uint8_t move_velocity_cms = (uint8_t)(move_distance_cm / time_s);// v = l/t
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

//**********************Calibration****************************
//Quick calibration
uint8_t Quick_Calibration(void)
{
	if (dt.wait_ck == 0) //there is no wait ack
	{
		dt.cmd_send.CID = 0x01;
		dt.cmd_send.CMD[0] = 0x00;
		dt.cmd_send.CMD[1] = 0x03;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//MAG calibration
uint8_t Mag_Calibration(void)
{
	if (dt.wait_ck == 0) //there is no wait ack
	{
		dt.cmd_send.CID = 0x01;
		dt.cmd_send.CMD[0] = 0x00;
		dt.cmd_send.CMD[1] = 0x04;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//SixAxis calibration
uint8_t SixAxis_Calibration(void)
{
	if (dt.wait_ck == 0) //there is no wait ack
	{
		dt.cmd_send.CID = 0x01;
		dt.cmd_send.CMD[0] = 0x00;
		dt.cmd_send.CMD[1] = 0x05;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

void heightcontrol(int dst, int pos)
{
	const int P = 1;
	static int speed;
	speed = P * (dst - pos);
	if(speed < 0)
	{
		if(speed < -5) speed = -5;
		Height_Move_Down(dst, -speed);
	}
	else
	{
		if(speed > 5) speed = 5;
		Height_Move_Down(dst, speed);
	}
}

//高度调整
u8 Height_Move_Up(u16 distance_cm, u16 velocity_cmps)
{
	//
	if (dt.wait_ck == 0)
	{
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X01;
		//
		dt.cmd_send.CMD[2] = BYTE0(distance_cm);
		dt.cmd_send.CMD[3] = BYTE1(distance_cm);
		dt.cmd_send.CMD[4] = BYTE0(velocity_cmps);
		dt.cmd_send.CMD[5] = BYTE1(velocity_cmps);
		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 Height_Move_Down(u16 distance_cm, u16 velocity_cmps)
{
	//
	if (dt.wait_ck == 0)
	{
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X02;
		//
		dt.cmd_send.CMD[2] = BYTE0(distance_cm);
		dt.cmd_send.CMD[3] = BYTE1(distance_cm);
		dt.cmd_send.CMD[4] = BYTE0(velocity_cmps);
		dt.cmd_send.CMD[5] = BYTE1(velocity_cmps);
		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}