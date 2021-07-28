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
	uint32_t ImgSize;		//ͼ�����ݵĴ�С
	uint8_t *pBuff;			//ͼ������
	uint32_t nImageType;	//OB_PIXEL_FORMAT_YUV422 �� OB_PIXEL_FORMAT_MJPEG
}TImageData;

#define UVC_BUFF_NUM 2
#define MAX_UVC_WIDTH 1920
#define MAX_UVC_HEIGHT 1080

//���֧�ֶ���1280x1080��YUV422��UVC����
#define MAX_BUFFER_SIZE (MAX_UVC_WIDTH*MAX_UVC_HEIGHT*2)



/*
  UVCSwapper�⣺ʹ��˵����
  1. UVCSwapper��֧��MJPEG��YUV422��UVC��ʽ���ݣ�����Ҫ������ʽ����Ҫ�޸�ObUvcAPIWin32.cpp����;
  2. UVCSwapper ��ȡ��������Jpeg��yuv422(YUYV)��ʽ���ݣ���Ҫ��Ӧ�ó���ת����������ʽ;
*/
class  ORBBEC_DLL_API UVCSwapper
{
public:
	UVCSwapper();
	~UVCSwapper();

	//�������ܣ���ʼ��UVC
	//����������
	//[in] width: ͼ��Ŀ�;
	//[in] height: ͼ��ĸ�;
	//[in] imageFormat: ͼ���ʽ (OB_PIXEL_FORMAT_YUV422 ��OB_PIXEL_FORMAT_MJPEG)
	int UvcInit(int width, int height, int imageFormat);

	//����ʼ��
	int UvcDeInit(int subdevice_index =0);

	//�������ܣ�����UVC��
	//����������
	//[in] subdevice_index: ����Ҫ���ã�ʹ��Ĭ��ֵ��

	int UVCStreamStart(int subdevice_index =0);

	//ֹͣUVC��
	int UVCStreamStop(int subdevice_index =0);

	//��ȡUVC����
	//����������
	//[in,out] pBuff:����uvc���ݵĻ�����,�ɵ����߷����ڴ�;
	//[out] nSize: uvc ���ݵ�size;
	//[out] imageType :ͼ���������ͣ�(OB_PIXEL_FORMAT_YUV422 ��OB_PIXEL_FORMAT_MJPEG);
	//[in] nTimeout:��ʱʱ��;
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

	//ͼ������
	int m_imageType;
	HANDLE m_hEvent;



};

