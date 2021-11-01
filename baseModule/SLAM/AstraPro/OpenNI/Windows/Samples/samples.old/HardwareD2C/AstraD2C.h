#pragma once
#include "d2cSwapper.h"
#include "ObCommon.h"

class AstraD2C :public d2cSwapper
{
public:
	AstraD2C();
	virtual ~AstraD2C();

	//函数功能：相机初始化
	int CameraInit(int d2cType);

	//函数功能：相机反初始化
	int CameraUnInit(void);

	//函数功能：获取相机流数据
	int GetStreamData(cv::Mat &cv_rgb, cv::Mat &cv_depth);

	//函数功能：停止流
	int StreamStop(void);

	//函数功能: 获取相机的内外参
	//参数：
	//[out] cameraParam: 相机的内外参
	//返回值：0:表示OK; 非0表示获取参数失败
	int GetCameraParam(OBCameraParams &cameraParam);

	//函数功能：获取Depth分辨率
	//参数：
	//出参[Out] nImageWidth: 图像宽;
	//出参[Out] nImageHeight: 图像高;
	//返回值：成功返回 CAMERA_STATUS_SUCCESS，失败返回 CAMERA_STATUS_DEPTH_GET_RESOLUTION_FAIL
	int GetCameraResolution(int &nImageWidth, int &nImageHeight);

	//函数功能：获取设备的pid
	uint16_t GetDevicePid(void);

private:

	/**** start depth swapper ****/
	int DepthInit(void);
	int DepthUnInit();

	int Depthstart(int width, int height);
	int Depthstop();
	int WaitDepthStream(VideoFrameRef &frame);
	void CalcDepthHist(VideoFrameRef& frame);
	bool IsLunaDevice(void);

	//depth data
	Device m_device;

	VideoStream m_depthStream;
	//openni::VideoFrameRef m_depthFrame;
	bool m_bDepthInit;
	bool m_bDepthStart;

	bool m_bDepStreamCreate;


	float* m_histogram;
	int m_ImageWidth;
	int m_ImageHeight;

	/***end depth swapper********/

	/****start color swapper****/
	int ColorStart(int width, int height);
	int ColorStop();
	int WaitColorStream(VideoFrameRef &frame);

	VideoStream m_ColorStream;
	//openni::VideoFrameRef m_ColorFrame;
	bool m_bColorStart;
	bool m_bColorStreamCreate;
	/***end color swapper ******/

private:

	
};

