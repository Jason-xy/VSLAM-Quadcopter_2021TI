#ifndef __MOVECONTROL_H__
#define __MOVECONTROL_H__
#include "SysConfig.h"
#include "LX_FC_Fun.h"
#include "ANO_DT_LX.h"
#include "LX_FC_State.h"
#include "Ano_math.h"

//V-SLAM
static int16_t t265_x_velocity_cmps, t265_y_velocity_cmps, t265_z_velocity_cmps;
static int16_t t265_x_position, t265_y_position, t265_z_position;

//MoveControl Get OneByte from USART2
void MoveControl_GetOneByte(uint8_t data);
void MoveControl_DataAnl(uint8_t *data, uint8_t len);
void MoveControl_Output(void);
uint8_t MoveControl_Spin(uint16_t spin_angle_0_360, uint16_t spin_speed_dps);

//Quick calibration
uint8_t Quick_Calibration(void);

#endif //__MOVECONTROL_H__
