#pragma once

#ifdef ORBBEC_DLL_API
#define ORBBEC_DLL_API _declspec(dllexport)
#else
#define ORBBEC_DLL_API _declspec(dllimport)
#endif

#include "windows.h"
#include "ObUvcApi.h"
#include "ObCommon.h"

#include <list>
using namespace std;
#define ORBBEC_VID 0x2bc5

typedef struct _ImageData {
	uint32_t ImgSize;		//图像数据的大小
	uint8_t *pBuff;			//图像数据
	uint32_t nImageType;	//OB_PIXEL_FORMAT_YUV422 和 OB_PIXEL_FORMAT_MJPEG
}TImageData;

#define UVC_BUFF_NUM 2
#define MAX_UVC_WIDTH 1920
#define MAX_UVC_HEIGHT 1080

//最大支持读出1280x1080的YUV422的UVC数据
#define MAX_BUFFER_SIZE (MAX_UVC_WIDTH*MAX_UVC_HEIGHT*2)



/*
  UVCSwapper库：使用说明：
  1. UVCSwapper库支持MJPEG和YUV422的UVC格式数据，如需要其它格式，需要修改ObUvcAPIWin32.cpp代码;
  2. UVCSwapper 获取的数据是Jpeg或yuv422(YUYV)格式数据，需要在应用程序转换成其它格式;
*/
class  ORBBEC_DLL_API UVCSwapper
{
public:
	UVCSwapper();
	~UVCSwapper();

	//函数功能：初始化UVC
	//函数参数：
	//[in] width: 图像的宽;
	//[in] height: 图像的高;
	//[in] imageFormat: 图像格式 (OB_PIXEL_FORMAT_YUV422 或OB_PIXEL_FORMAT_MJPEG)
	int UvcInit(int width, int height, int imageFormat);

	//反初始化
	int UvcDeInit(int subdevice_index =0);

	//函数功能：开启UVC流
	//函数参数：
	//[in] subdevice_index: 不需要设置，使用默认值。

	int UVCStreamStart(int subdevice_index =0);

	//停止UVC流
	int UVCStreamStop(int subdevice_index =0);

	//获取UVC数据
	//函数参数：
	//[in,out] pBuff:保存uvc数据的缓冲区,由调用者分配内存;
	//[out] nSize: uvc 数据的size;
	//[out] imageType :图像数据类型，(OB_PIXEL_FORMAT_YUV422 或OB_PIXEL_FORMAT_MJPEG);
	//[in] nTimeout:超时时间;
	int WaitUvcStream(uint8_t* pBuff, uint32_t &nSize, uint32_t &imageType, uint32_t nTimeout);

private:
	static void ProcessorCallback(const void *frame, int size, void *pstream);
	void ImageProcess(const void *frame, int size);
	bool m_bUvcInit;
	typedef std::map<std::string, std::shared_ptr<ObUVCDevice>> ObUVCDeviceMap;
	ObUVCDeviceMap uvcDevices;
	std::shared_ptr < ObUVCDevice > m_pDev;
	ObSensorInfos m_ObsensorInfos;
	//
	CRITICAL_SECTION m_csLock;
	std::list<TImageData> m_FrameList;
	std::list<TImageData> m_FreeList;
	bool m_bUvcStart;

	//图像类型
	int m_imageType;
	HANDLE m_hEvent;



};

