/*==========================================================================
 * 描述    ：凌霄飞控状态处理（主要是摇杆解锁，触发校准等）
 * 更新时间：2020-03-31 
 * 作者		 ：匿名科创-Jyoun
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 * 项目合作：18084888982，18061373080
============================================================================
 * 匿名科创团队感谢大家的支持，欢迎大家进群互相交流、讨论、学习。
 * 若您觉得匿名有不好的地方，欢迎您拍砖提意见。
 * 若您觉得匿名好，请多多帮我们推荐，支持我们。
 * 匿名开源程序代码欢迎您的引用、延伸和拓展，不过在希望您在使用时能注明出处。
 * 君子坦荡荡，小人常戚戚，匿名坚决不会请水军、请喷子，也从未有过抹黑同行的行为。  
 * 开源不易，生活更不容易，希望大家互相尊重、互帮互助，共同进步。
 * 只有您的支持，匿名才能做得更好。  
===========================================================================*/
#include "LX_FC_State.h"
#include "Drv_RcIn.h"
#include "ANO_DT_LX.h"
#include "LX_FC_Fun.h"

_fc_state_st fc_sta;
_sticks_fun_st sti_fun;

//
static u16 time_dly_cnt_ms;
static u8 unlock_lock_flag;

static void LX_Unlock_Lock_Check(float *dT_s)
{
	//判断yaw摇杆是否大致回中
	if ((rc_in.rc_ch.st_data.ch_[ch_4_yaw] > 1400 && rc_in.rc_ch.st_data.ch_[ch_4_yaw] < 1600))
	{
		sti_fun.stick_mit_pos = 1;
		unlock_lock_flag = 1; //回中以后才能执行一次解锁或者上锁
	}
	else
	{
		sti_fun.stick_mit_pos = 0;
	}
	//标记预备上锁的动作
	if (rc_in.rc_ch.st_data.ch_[ch_3_thr] < 1200 && (sti_fun.stick_mit_pos == 0))
	{
		sti_fun.pre_locking = 1;
	}
	else
	{
		sti_fun.pre_locking = 0;
	}
	//解锁
	if (unlock_lock_flag == 1) //执行条件
	{
		if (UNLOCK_STATE == 0)
		{
			if (STICKS_UNLOCK_REQ)
			{
				if (time_dly_cnt_ms < UNLOCK_HOLD_TIME_MS)
				{
					time_dly_cnt_ms += *(dT_s)*1e3f;
				}
				else
				{
					FC_Unlock(); //解锁
					time_dly_cnt_ms = 0;
					unlock_lock_flag = 0; //不再执行
				}
			}
			else
			{
				time_dly_cnt_ms = 0;
			}
		}
		else if (UNLOCK_STATE == 1)
		{
			if (STICKS_LOCK_REQ)
			{
				if (time_dly_cnt_ms < LOCK_HOLD_TIME_MS)
				{
					time_dly_cnt_ms += *(dT_s)*1e3f;
				}
				else
				{
					FC_Lock(); //上锁
					time_dly_cnt_ms = 0;
					unlock_lock_flag = 0; //不再执行
				}
			}
			else
			{
				time_dly_cnt_ms = 0;
			}
		}
	}
	else
	{
		//null
	}
}

void LX_Cali_Trig_Check()
{
	static u8 cali_f;
	//为上锁状态才执行
	if (UNLOCK_STATE == 0)
	{
		//执行条件
		if (STICKS_CALI_HOR_REQ)
		{
			//标记只执行一次
			if (cali_f == 0)
			{
				Horizontal_Calibrate();
				cali_f = 1;
			}
		}
		else if (STICKS_CALI_MAG_REQ)
		{
			if (cali_f == 0)
			{
				Mag_Calibrate();
				cali_f = 1;
			}
		}
		else
		{
			cali_f = 0;
		}
	}
}

void LX_FC_State_Task(float dT_s)
{
	//有遥控信号才执行
	if (rc_in.no_signal == 0)
	{
		//
		LX_Unlock_Lock_Check(&dT_s);
		//
		LX_Cali_Trig_Check();
	}
}
