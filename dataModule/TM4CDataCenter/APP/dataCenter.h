#ifndef __DATACENTER_H__
#define __DATACENTER_H__
#include "SysConfig.h"
#include "LX_FC_Fun.h"
#include "ANO_DT_LX.h"
#include "LX_FC_State.h"
#include "Ano_math.h"
#include "Drv_Uart.h"
#include <stdlib.h>
#include <string.h>
#include "laserControl.h"


//Jetson
//Location
extern int16_t t265_x_position_control;
extern int16_t t265_y_position_control;
extern uint8_t state_is_land;
extern uint8_t barcode_data;






//TM4C-USART0->U1 //BAN
//U6 PB0 PB1 RX TX
//U5 PD6 PD7
//U2 PC4 PC5
//U3 PE4 PE5
//U4 PE0 PE1
//U7 PC6 PC7

//dataCenter:0x62
//U2
//U2 PC4 PC5 RX TX
void MainBoard_GetOneByte(uint8_t data);
void MainBoard_DataAnl(uint8_t *data, uint8_t len);
void MainBoard_BeginSixSidesCalibration(void);
void MainBoard_BeginCompassCalibration(void);
void MainBoard_BeginLevelCalibration(void);
void MainBoard_Unlock(void);
void MainBoard_OneKeyLand(void);
void check_color(void);

//U3
//OpenMV:0x63
//RX:B11
//TX:B10
//U3 PE4 PE5 RX TX
//function��0xa2 0xa3 0xa4
void OpenMV_GetOneByte(uint8_t data);
void OpenMV_DataAnl(uint8_t *data, uint8_t len);
void OpenMV_RequestLAB(void);
void OpenMV_SetLAB(void);
//U4
void K210_GetOneByte(uint8_t data);
void K210_DataAnl(uint8_t *data, uint8_t len);
//U5
void Jetson_GetOneByte(uint8_t data);
void Jetson_DataAnl(uint8_t *data, uint8_t len);
void send_t265_data(void);
void sent_Control_ToANO(uint8_t *data, uint8_t len);
extern int8_t blink_state;
//U6
//BarcodeScanner
void BarcodeScanner_GetOneByte(uint8_t data);
void BarcodeScanner_DataAnl(uint8_t *data, uint8_t len);
void Barcode_Blink_Init(void);
void Barcode_Blink(void);

//U7
//Screen:0x67
//U7 PC6 PC7 RX TX
void Screen_GetOneByte(uint8_t data);
void Screen_DataAnl(uint8_t *data, uint8_t len);
void Screen_BackData(void);

#endif //__MOVECONTROL_H__