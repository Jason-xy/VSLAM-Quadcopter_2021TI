#ifndef __MOVECONTROL_H__
#define __MOVECONTROL_H__
#include "SysConfig.h"
#include "LX_FC_Fun.h"
#include "ANO_DT_LX.h"
#include "LX_FC_State.h"
#include "Ano_math.h"

//MoveControl Get OneByte from USART2
void MoveControl_GetOneByte(uint8_t data);
void MoveControl_DataAnl(uint8_t *data, uint8_t len);
void MoveControl_Output(void);
uint8_t MoveControl_Spin(uint16_t spin_angle_0_360, uint16_t spin_speed_dps);

void OVControl_GetOneByte(uint8_t data);
void OVControl_DataAnl(uint8_t *data, uint8_t len);

#endif //__MOVECONTROL_H__
