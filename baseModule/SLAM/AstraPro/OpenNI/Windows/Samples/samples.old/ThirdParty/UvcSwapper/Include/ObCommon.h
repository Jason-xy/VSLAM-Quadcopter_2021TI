#ifndef __OB_COMMON_H__
#define __OB_COMMON_H__


//#include <XnStatus.h>
#include <iostream>


#define OB_LOG_INFO(args)   printf("OB_INFO : ");printf(args)
#define OB_LOG_ERROR(args)   printf("OB_ERROR : ");printf(args)

typedef enum
{
	CAMERA_STATUS_SUCCESS = 0,
	CAMERA_STATUS_DEPTH_INIT_FAIL = 1,	//depth ��ʼ��ʧ��
	CAMERA_STATUS_DEPTH_OPEN_FAIL = 2,	//depth openʧ��
	CAMERA_STATUS_DEPTH_CREATE_FAIL = 3, //depth create��ʧ��
	CAMERA_STATUS_DEPTH_START_FAIL = 4,	//depth start��ʧ��
	CAMERA_STATUS_DEPTH_WAIT_TIMEOUT = 5,  //depth�ȴ�����ʱ
	CAMERA_STATUS_GET_DEPTH_FAIL = 6,	//��ȡdepth��ʧ��
	CAMERA_STATUS_GET_RGB_FAIL = 7,		//��ȡrgb��ʧ��
	CAMERA_STATUS_RGB_INIT_FAIL = 8,	//RGB��ʼ��ʧ��
	CAMERA_STATUS_DEPTH_DISCONNECT = 9,  //depth �Ͽ�����
	CAMERA_STATUS_DEVICE_NOT_FOUND = 10, //δ�ҵ�ʧ��
	CAMERA_STATUS_RESOLUTION_UPSUPPORT = 11, //�ֱ��ʲ�֧��
	CAMERA_STATUS_DEPTH_GET_CAMERA_PARAM_FAIL = 12, //��ȡ�����ʧ��
	CAMERA_STATUS_DEPTH_GET_RESOLUTION_FAIL = 13,  //��ȡDepth�ֱ���ʧ��
	CAMERA_STATUS_RGB_CREATE_FAIL = 14, //color create��ʧ��
	CAMERA_STATUS_RGB_START_FAIL = 15,	//color start��ʧ��
	CAMERA_STATUS_RGB_WAIT_TIMEOUT = 16,  //color�ȴ�����ʱ


} ObCameraStatus;

#endif // __OB_COMMON_H__