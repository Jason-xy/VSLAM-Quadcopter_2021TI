#pragma once
#include <opencv2/opencv.hpp>
#include "OpenNI.h"
using namespace openni;

#define DEPTH_TIMEOUT 500
#define UVC_TIME_OUT 3000

//start astra device pid
#define AstraPID_0401 0x0401
#define AstraPID_0402 0x0402
#define AstraPID_0404 0x0404

#define AstraPID_0407 0x0407
#define AstraPID_0408 0x0408
//end astra device pid

//start uvc device pid 
//640x480
#define ASTRAPRO_PID 0x0403
#define ButterFly_PID 0x0618
#define AstraProPlus_PID 0x060F
#define ASTRAPLUS_PID 0x0632

//400x640
#define ATLAS_PID 0x0613
//#define ASTRA_D2_PID 0x060d

//640x400
#define DEEYEA_PID 0x060b
#define Canglong_PID 0x0608
#define LunaP2_PID 0x0609
#define DaiBai_PID 0x060e

#define BUS_CL_PID 0x0610	//bus not have rgb sensor
#define ASTRA_BUS_PID 0x0611 //bus not have rgb sensor
#define GEMINI_PID 0x0614
#define Projector_PID 0x0617

//end uvc device pid

#define IMAGE_WIDTH_640 640
#define IMAGE_HEIGHT_480 480
#define IMAGE_HEIGHT_400 400

#define IMAGE_WIDTH_480 480
#define IMAGE_WIDTH_400 400
#define IMAGE_HEIGHT_640 640

typedef enum
{
	HARDWARE_D2C = 0,
	SOFTWARE_D2C = 1,
} ObD2CType;

class d2cSwapper
{
public:
	d2cSwapper();
	virtual ~d2cSwapper();

public:
	//函数功能：相机初始化
	virtual int CameraInit(int d2cType) = 0;

	//函数功能：相机反初始化
	virtual int CameraUnInit(void) =0;

	//函数功能：获取相机流数据
	virtual int GetStreamData(cv::Mat &cv_rgb, cv::Mat &cv_depth) =0;

	//函数功能：停止流
	virtual int StreamStop(void) =0;

	//函数功能: 获取相机的内外参
	//参数：
	//[out] cameraParam: 相机的内外参
	//返回值：0:表示OK; 非0表示获取参数失败
	virtual int GetCameraParam(OBCameraParams &cameraParam) =0 ;

	//函数功能：获取Depth分辨率
	//参数：
	//出参[Out] nImageWidth: 图像宽;
	//出参[Out] nImageHeight: 图像高;
	//返回值：成功返回 CAMERA_STATUS_SUCCESS，失败返回 CAMERA_STATUS_DEPTH_GET_RESOLUTION_FAIL
	virtual int GetCameraResolution(int &nImageWidth, int &nImageHeight) =0;

	//函数功能：获取设备的pid
	virtual uint16_t GetDevicePid(void) =0;
protected:
	int m_bD2cType;
};

