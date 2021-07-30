#ifndef __LX_FC_STATE_H
#define __LX_FC_STATE_H

//==引用
#include "SysConfig.h"

//宏定义
//美国手，外八
#define TOE_OUT \
	(rc_in.rc_ch.st_data.ch_[ch_1_rol] > 1900 && rc_in.rc_ch.st_data.ch_[ch_2_pit] < 1100 && rc_in.rc_ch.st_data.ch_[ch_3_thr] < 1100 && rc_in.rc_ch.st_data.ch_[ch_4_yaw] < 1100)

//美国手，内八
#define TOE_IN \
	(rc_in.rc_ch.st_data.ch_[ch_4_yaw] > 1900 && rc_in.rc_ch.st_data.ch_[ch_2_pit] < 1100 && rc_in.rc_ch.st_data.ch_[ch_3_thr] < 1100 && rc_in.rc_ch.st_data.ch_[ch_1_rol] < 1100)

//美国手，左下+左下
#define LD_LD \
	(rc_in.rc_ch.st_data.ch_[ch_4_yaw] < 1100 && rc_in.rc_ch.st_data.ch_[ch_2_pit] < 1100 && rc_in.rc_ch.st_data.ch_[ch_3_thr] < 1100 && rc_in.rc_ch.st_data.ch_[ch_1_rol] < 1100)

//美国手，右下+右下
#define RD_RD \
	(rc_in.rc_ch.st_data.ch_[ch_4_yaw] > 1900 && rc_in.rc_ch.st_data.ch_[ch_2_pit] < 1100 && rc_in.rc_ch.st_data.ch_[ch_3_thr] < 1100 && rc_in.rc_ch.st_data.ch_[ch_1_rol] > 1900)

// 摇杆触发校准水平条件
#define STICKS_CALI_HOR_REQ (LD_LD)
// 摇杆触发校准罗盘条件
#define STICKS_CALI_MAG_REQ (RD_RD)

// 摇杆解锁条件requirement
#define STICKS_UNLOCK_REQ (TOE_OUT || TOE_IN)
// 摇杆上锁条件
#define STICKS_LOCK_REQ (STICKS_UNLOCK_REQ)
// 解锁持续时间,毫秒
#define UNLOCK_HOLD_TIME_MS (1000)
// 上锁持续时间,毫秒
#define LOCK_HOLD_TIME_MS (300)
// 当前解锁/上锁状态
#define UNLOCK_STATE (fc_sta.unlock_sta)

//==定义/声明
typedef struct
{
	u8 pre_locking;
	u8 stick_mit_pos;

} _sticks_fun_st;

typedef struct
{
	u8 CID;
	u8 CMD_0;
	u8 CMD_1;
} _cmd_fun_st;
//飞控状态
typedef struct
{
	//模式
	u8 fc_mode_cmd;
	u8 fc_mode_sta;

	//解锁上锁
	u8 unlock_cmd;
	u8 unlock_sta;

	//指令功能
	_cmd_fun_st cmd_fun;

	//state
	u8 imu_ready;
	u8 take_off;
	u8 in_air;
	u8 landing;

} _fc_state_st;

//==数据声明
extern _fc_state_st fc_sta;
extern _sticks_fun_st sti_fun;
//==函数声明
//static

//public
void LX_FC_State_Task(float dT_s);

#endif
