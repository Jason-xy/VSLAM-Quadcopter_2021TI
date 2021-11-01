#ifndef _REMOTE_CONTROLLER_H
#define _REMOTE_CONTROLLER_H

#include "sys.h"
#include "usart.h"


/*
��Ҫʵ�ֵĹ���

���ͣ�
1.ACCУ׼
2.GYROУ׼
3.����ˮƽУ׼
4.���ٶ�����У׼

5. ����
6.����������ͣ��
����
1.����log�ַ���
2.��������У��֡

*/

//����2����
void sent_CMD_ACC_calibration(void);
void sent_CMD_GYRO_calibration(void);
void sent_CMD_RH_calibration(void);//Rapid horizontal calibration
void sent_CMD_MAG_calibration(void);
void sent_CMD_SixSides_calibration(void);
void sent_CMD_unlock(void);
void sent_CMD_Estop(void);


//����1����
void sent_utf8_txt(u8* data,u8 len);

#endif

