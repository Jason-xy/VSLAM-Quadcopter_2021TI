#include "MoveControl.h"

static uint8_t MoveControl_datatemp[50];
static uint16_t distance_cm, velocity_cmps, dir_angle_0_360, spin_angle_0_360;
//MoveControl Get OneByte from USART2
void MoveControl_GetOneByte(uint8_t data){
    static u8 _data_len = 0, _data_cnt = 0;
	static u8 rxstate = 0;

	if (rxstate == 0 && data == 0xAA)
	{
        //HEAD
		rxstate = 1;
		MoveControl_datatemp[0] = data;
	}
	else if (rxstate == 1 && (data == HW_TYPE || data == HW_ALL))
	{
        //Dest
		rxstate = 2;
		MoveControl_datatemp[1] = data;
	}
	else if (rxstate == 2)
	{
        //Func
		rxstate = 3;
		MoveControl_datatemp[2] = data;
	}
	else if (rxstate == 3 && data < 250)
	{
        //Len
		rxstate = 4;
		MoveControl_datatemp[3] = data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if (rxstate == 4 && _data_len > 0)
	{
        //data
		_data_len--;
		MoveControl_datatemp[4 + _data_cnt++] = data;
		if (_data_len == 0)
			rxstate = 5;
	}
	else if (rxstate == 5)
	{
        //SumCheck
		rxstate = 6;
		MoveControl_datatemp[4 + _data_cnt++] = data;
	}
	else if (rxstate == 6)
	{
        //add on check
		rxstate = 0;
		MoveControl_datatemp[4 + _data_cnt] = data;
		//DT_data_cnt = _data_cnt+5;
    //data resolution
		MoveControl_DataAnl(MoveControl_datatemp, _data_cnt + 5);
	}
	else
	{
        //error
		rxstate = 0;
	}
}

//MoveControl data resolution
void MoveControl_DataAnl(uint8_t *data, uint8_t len){

    //data check
    uint8_t check_sum1 = 0, check_sum2 = 0;
	if (*(data + 3) != (len - 6)) //check the length of the data
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		check_sum1 += *(data + i);//Sum check
		check_sum2 += check_sum1;//add on check
	}
	if ((check_sum1 != *(data + len - 2)) || (check_sum2 != *(data + len - 1))) //compare the checksum
		return;
	/*================================================================================*/
    //Get data
    distance_cm = (*(data + 4) << 8) | (*(data + 5));
    velocity_cmps = (*(data + 6) << 8) | (*(data + 7));
    dir_angle_0_360 = (*(data + 8) << 8) | (*(data + 9));
    spin_angle_0_360 = (*(data + 10) << 8) | (*(data + 11));
}

static u8 mission_f = 0;
static u8 mission_step = 0;
static u16 time_dly_cnt_ms;
u16 control_stat = 1;
//MoveControl Output
void MoveControl_Output(void){
		if(mission_f == 0)
		{
			switch(mission_step)
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
					//unlock
					mission_step += FC_Unlock();
				}
				break;
				case 3:
				{
					if(time_dly_cnt_ms < 2000)
					{
						time_dly_cnt_ms += 50;//ms
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
					mission_step += OneKey_Takeoff(50);
				}
				break;
				case 5:
				{
					if(time_dly_cnt_ms < 10000)
					{
						time_dly_cnt_ms += 50;
					}
					else
					{
						time_dly_cnt_ms =0;
						mission_step++;
					}
				}
				break;
				case 6:
				{
//					if(fc_sta.unlock_cmd == 0)
//					{
					if(control_stat == 1)
					{
						Horizontal_Move(distance_cm, velocity_cmps, dir_angle_0_360);
						control_stat = 0;
					}
					else
					{
						control_stat =1;
						MoveControl_Spin(spin_angle_0_360);
					}
//					}
				}
			}
		}
}

//MoveControl Spin
static uint16_t spin_speed = 3;
uint8_t MoveControl_Spin(uint16_t spin_angle_0_360){
		if (dt.wait_ck == 0) //there is no wait ack
		{
				dt.cmd_send.CID = 0x10;
				dt.cmd_send.CMD[0] = 0x02;
				if(spin_angle_0_360 <= 180){
						dt.cmd_send.CMD[1] = 0x08;
				}else{
						spin_angle_0_360 = 360 - spin_angle_0_360;
						dt.cmd_send.CMD[1] = 0x07;
				}
				dt.cmd_send.CMD[2] = BYTE0(spin_angle_0_360);
				dt.cmd_send.CMD[3] = BYTE1(spin_angle_0_360);
				dt.cmd_send.CMD[4] = BYTE0(spin_speed);
				dt.cmd_send.CMD[5] = BYTE1(spin_speed);
				CMD_Send(0xff, &dt.cmd_send);
				return 1;
		}
		else
		{
			return 0;
		}
}
