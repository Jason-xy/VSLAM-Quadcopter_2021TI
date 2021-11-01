#ifndef _REMOTE_CONTROLLER_H
#define _REMOTE_CONTROLLER_H

#include "sys.h"
#include "usart.h"


/*
需要实现的功能

发送：
1.ACC校准
2.GYRO校准
3.快速水平校准
4.加速度六面校准

5. 解锁
6.锁定、紧急停机
接收
1.接收log字符串
2.接收数据校验帧

*/

//串口2发送
void sent_CMD_ACC_calibration(void);
void sent_CMD_GYRO_calibration(void);
void sent_CMD_RH_calibration(void);//Rapid horizontal calibration
void sent_CMD_MAG_calibration(void);
void sent_CMD_SixSides_calibration(void);
void sent_CMD_unlock(void);
void sent_CMD_Estop(void);


//串口1发送
void sent_utf8_txt(u8* data,u8 len);

#endif

