#ifndef __OB_COMMON_H__
#define __OB_COMMON_H__


//#include <XnStatus.h>
#include <iostream>


#define OB_LOG_INFO(args)   printf("OB_INFO : ");printf(args)
#define OB_LOG_ERROR(args)   printf("OB_ERROR : ");printf(args)

typedef enum
{
	CAMERA_STATUS_SUCCESS = 0,
	CAMERA_STATUS_DEPTH_INIT_FAIL = 1,	//depth 初始化失败
	CAMERA_STATUS_DEPTH_OPEN_FAIL = 2,	//depth open失败
	CAMERA_STATUS_DEPTH_CREATE_FAIL = 3, //depth create流失败
	CAMERA_STATUS_DEPTH_START_FAIL = 4,	//depth start流失败
	CAMERA_STATUS_DEPTH_WAIT_TIMEOUT = 5,  //depth等待流超时
	CAMERA_STATUS_GET_DEPTH_FAIL = 6,	//获取depth流失败
	CAMERA_STATUS_GET_RGB_FAIL = 7,		//获取rgb流失败
	CAMERA_STATUS_RGB_INIT_FAIL = 8,	//RGB初始化失败
	CAMERA_STATUS_DEPTH_DISCONNECT = 9,  //depth 断开连接
	CAMERA_STATUS_DEVICE_NOT_FOUND = 10, //未找到失败
	CAMERA_STATUS_RESOLUTION_UPSUPPORT = 11, //分辨率不支持
	CAMERA_STATUS_DEPTH_GET_CAMERA_PARAM_FAIL = 12, //获取内外参失败
	CAMERA_STATUS_DEPTH_GET_RESOLUTION_FAIL = 13,  //获取Depth分辨率失败
	CAMERA_STATUS_RGB_CREATE_FAIL = 14, //color create流失败
	CAMERA_STATUS_RGB_START_FAIL = 15,	//color start流失败
	CAMERA_STATUS_RGB_WAIT_TIMEOUT = 16,  //color等待流超时


} ObCameraStatus;

#endif // __OB_COMMON_H__