#ifndef __LX_FC_STATE_H
#define __LX_FC_STATE_H

//==����
#include "SysConfig.h"

//�궨��
//�����֣����
#define TOE_OUT \
	(rc_in.rc_ch.st_data.ch_[ch_1_rol] > 1900 && rc_in.rc_ch.st_data.ch_[ch_2_pit] < 1100 && rc_in.rc_ch.st_data.ch_[ch_3_thr] < 1100 && rc_in.rc_ch.st_data.ch_[ch_4_yaw] < 1100)

//�����֣��ڰ�
#define TOE_IN \
	(rc_in.rc_ch.st_data.ch_[ch_4_yaw] > 1900 && rc_in.rc_ch.st_data.ch_[ch_2_pit] < 1100 && rc_in.rc_ch.st_data.ch_[ch_3_thr] < 1100 && rc_in.rc_ch.st_data.ch_[ch_1_rol] < 1100)

//�����֣�����+����
#define LD_LD \
	(rc_in.rc_ch.st_data.ch_[ch_4_yaw] < 1100 && rc_in.rc_ch.st_data.ch_[ch_2_pit] < 1100 && rc_in.rc_ch.st_data.ch_[ch_3_thr] < 1100 && rc_in.rc_ch.st_data.ch_[ch_1_rol] < 1100)

//�����֣�����+����
#define RD_RD \
	(rc_in.rc_ch.st_data.ch_[ch_4_yaw] > 1900 && rc_in.rc_ch.st_data.ch_[ch_2_pit] < 1100 && rc_in.rc_ch.st_data.ch_[ch_3_thr] < 1100 && rc_in.rc_ch.st_data.ch_[ch_1_rol] > 1900)

// ҡ�˴���У׼ˮƽ����
#define STICKS_CALI_HOR_REQ (LD_LD)
// ҡ�˴���У׼��������
#define STICKS_CALI_MAG_REQ (RD_RD)

// ҡ�˽�������requirement
#define STICKS_UNLOCK_REQ (TOE_OUT || TOE_IN)
// ҡ����������
#define STICKS_LOCK_REQ (STICKS_UNLOCK_REQ)
// ��������ʱ��,����
#define UNLOCK_HOLD_TIME_MS (1000)
// ��������ʱ��,����
#define LOCK_HOLD_TIME_MS (300)
// ��ǰ����/����״̬
#define UNLOCK_STATE (fc_sta.unlock_sta)

//==����/����
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
//�ɿ�״̬
typedef struct
{
	//ģʽ
	u8 fc_mode_cmd;
	u8 fc_mode_sta;

	//��������
	u8 unlock_cmd;
	u8 unlock_sta;

	//ָ���
	_cmd_fun_st cmd_fun;

	//state
	u8 imu_ready;
	u8 take_off;
	u8 in_air;
	u8 landing;

} _fc_state_st;

//==��������
extern _fc_state_st fc_sta;
extern _sticks_fun_st sti_fun;
//==��������
//static

//public
void LX_FC_State_Task(float dT_s);

#endif
