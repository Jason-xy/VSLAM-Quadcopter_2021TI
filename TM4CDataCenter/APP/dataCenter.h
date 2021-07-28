#ifndef __DATACENTER_H__
#define __DATACENTER_H__
#include "SysConfig.h"
#include "LX_FC_Fun.h"
#include "ANO_DT_LX.h"
#include "LX_FC_State.h"
#include "Ano_math.h"
#include "Drv_Uart.h"

//U2
void MainBoard_GetOneByte(uint8_t data);
void MainBoard_DataAnl(uint8_t *data, uint8_t len);
//U3
void OpenMV_GetOneByte(uint8_t data);
void OpenMV_DataAnl(uint8_t *data, uint8_t len);
//U4
void K210_GetOneByte(uint8_t data);
void K210_DataAnl(uint8_t *data, uint8_t len);
//U5
void Jetson_GetOneByte(uint8_t data);
void Jetson_DataAnl(uint8_t *data, uint8_t len);
//U6
void TOF_GetOneByte(uint8_t data);
void TOF_DataAnl(uint8_t *data, uint8_t len);
//U7
void Screen_GetOneByte(uint8_t data);
void Screen_DataAnl(uint8_t *data, uint8_t len);

#endif //__MOVECONTROL_H__