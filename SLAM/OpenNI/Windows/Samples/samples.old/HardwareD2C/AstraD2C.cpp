#include "AstraD2C.h"
#include "PS1080.h"
#include <string>
#include <algorithm>
using namespace std;

AstraD2C::AstraD2C()
{

	m_ImageWidth = IMAGE_WIDTH_640;
	m_ImageHeight = IMAGE_HEIGHT_480;

	m_bDepthInit = false;
	m_bDepthStart = false;
	m_bDepStreamCreate = false;

	m_bColorStart = false;
	m_bColorStreamCreate = false;
	m_bD2cType = HARDWARE_D2C;
}


AstraD2C::~AstraD2C()
{

}

int AstraD2C::DepthInit(void)
{

	openni::Status rc = OpenNI::initialize();
	if (rc != STATUS_OK)
	{
		return CAMERA_STATUS_DEPTH_INIT_FAIL;
	}

	rc = m_device.open(ANY_DEVICE);
	if (rc != STATUS_OK)
	{
		//
		OpenNI::shutdown();
		return CAMERA_STATUS_DEPTH_OPEN_FAIL;
	}

	m_bDepthInit = true;
	printf("depth init \n");

	return CAMERA_STATUS_SUCCESS;
}

int AstraD2C::DepthUnInit()
{
	Depthstop();
	ColorStop();
	if (m_bDepthInit)
	{
		m_device.close();
		OpenNI::shutdown();
	}

	m_bDepthInit = false;

	printf("depth deinit \n");
	return CAMERA_STATUS_SUCCESS;
}

int AstraD2C::Depthstart(int width, int height)
{
	if (m_bDepthStart)
	{
		return CAMERA_STATUS_SUCCESS;
	}

	openni::Status rc = openni::STATUS_OK;
	if (NULL == m_device.getSensorInfo(SENSOR_DEPTH))
	{
		return CAMERA_STATUS_DEPTH_CREATE_FAIL;
	}

	if (!m_bDepStreamCreate)
	{
		rc = m_depthStream.create(m_device, SENSOR_DEPTH);
		if (rc != STATUS_OK)
		{
			return CAMERA_STATUS_DEPTH_CREATE_FAIL;
		}

	}

	m_bDepStreamCreate = true;

	switch (m_bD2cType)
	{
	case HARDWARE_D2C:
		//hardware d2c must set d2c command
		m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
		break;
	case SOFTWARE_D2C:
		//soft d2c set depth not mirror
		m_depthStream.setMirroringEnabled(false);
		break;
	default:
		printf("ERROR: d2c type is not support\n");
		break;
	}


	if (width > 0 && height > 0)
	{
		VideoMode mode = m_depthStream.getVideoMode();
		mode.setResolution(width, height);
		m_depthStream.setVideoMode(mode);
	}

	rc = m_depthStream.start();
	if (rc != STATUS_OK)
	{
		return CAMERA_STATUS_DEPTH_START_FAIL;
	}

	m_bDepthStart = true;

	printf("depth start \n");

	return 0;
}

int AstraD2C::Depthstop()
{
	if (m_bDepthStart)
	{
		m_depthStream.stop();
	}

	m_bDepthStart = false;

	if (m_bDepStreamCreate)
	{
		m_depthStream.destroy();
	}
	m_bDepStreamCreate = false;

	printf("depth stop \n");

	return CAMERA_STATUS_SUCCESS;
}

int AstraD2C::WaitDepthStream(VideoFrameRef &frame)
{
	int changedIndex = -1;

	openni::Status rc = openni::STATUS_OK;

	openni::VideoStream* streams[] = { &m_depthStream };

	rc = OpenNI::waitForAnyStream(streams, 1, &changedIndex, DEPTH_TIMEOUT);
	if (rc == openni::STATUS_OK)
	{
		rc = m_depthStream.readFrame(&frame);
		if (rc != openni::STATUS_OK)
		{
			return CAMERA_STATUS_GET_DEPTH_FAIL;
		}
	}
	else
	{
		return CAMERA_STATUS_DEPTH_WAIT_TIMEOUT;
	}

	return CAMERA_STATUS_SUCCESS;
}


int AstraD2C::CameraInit(int d2cType)
{
	int nRet = DepthInit();
	m_bD2cType = d2cType;
	return nRet;
}


int AstraD2C::CameraUnInit(void)
{
	DepthUnInit();

	printf("CameraUnInit \n");
	return CAMERA_STATUS_SUCCESS;
}



int AstraD2C::GetStreamData(cv::Mat &cv_rgb, cv::Mat &cv_depth)
{
	DWORD nStartTime = GetTickCount();
	int nRet = -1;

	if (!m_bDepthInit)
	{
		nRet = DepthInit();
		if (nRet != CAMERA_STATUS_SUCCESS)
		{
			return nRet;
		}
	}

	//depth start
	if (!m_bDepthStart)
	{

		int nRet = GetCameraResolution(m_ImageWidth, m_ImageHeight);
		if (nRet != CAMERA_STATUS_SUCCESS)
		{
			return CAMERA_STATUS_DEPTH_GET_RESOLUTION_FAIL;
		}

		nRet = Depthstart(m_ImageWidth, m_ImageHeight);

		if (nRet != CAMERA_STATUS_SUCCESS)
		{
			return CAMERA_STATUS_DEPTH_START_FAIL;
		}
	}

	//color start
	if (!m_bColorStart)
	{

		//rgb 分辨率为 640x480
		nRet = ColorStart(IMAGE_WIDTH_640, IMAGE_HEIGHT_480);

		if (nRet != CAMERA_STATUS_SUCCESS)
		{
			return CAMERA_STATUS_DEPTH_START_FAIL;
		}
	}

	//wait depth stream
	VideoFrameRef frame;
	nRet = WaitDepthStream(frame);
	if (nRet == CAMERA_STATUS_SUCCESS)
	{
		if (frame.getWidth() == m_ImageWidth && frame.getHeight() == m_ImageHeight)
		{
			int nFrameSize = sizeof(OniDepthPixel) * frame.getWidth() * frame.getHeight();
			memcpy(cv_depth.data, frame.getData(), nFrameSize);
			cv_depth.cols = frame.getWidth();
			cv_depth.rows = frame.getHeight();
		}
		else
		{
			return CAMERA_STATUS_GET_DEPTH_FAIL;
		}
	}
	else
	{
		return nRet;
	}

	//wait color stream
	VideoFrameRef colorFrame;
	nRet = WaitColorStream(colorFrame);
	if (nRet == CAMERA_STATUS_SUCCESS)
	{
		if (colorFrame.getWidth() == IMAGE_WIDTH_640 && colorFrame.getHeight() == IMAGE_HEIGHT_480)
		{

			for (unsigned int y = 0; y < (IMAGE_HEIGHT_480); ++y)
			{
				uint8_t* data = (uint8_t*)cv_rgb.ptr<uchar>(y);
				for (unsigned int x = 0; x < IMAGE_WIDTH_640; ++x)
				{
					//rgb888 to bgr888
					OniRGB888Pixel* streamPixel = (OniRGB888Pixel*)((char*)colorFrame.getData() + ((int)(y) * colorFrame.getStrideInBytes())) + (int)(x);
					data[0] = streamPixel->b;
					data[1] = streamPixel->g;
					data[2] = streamPixel->r;
					data = data + 3;
				}
			}
		}
		else
		{
			return CAMERA_STATUS_GET_RGB_FAIL;
		}
	}
	else
	{
		return nRet;
	}

	return CAMERA_STATUS_SUCCESS;
}

int AstraD2C::StreamStop(void)
{
	Depthstop();
	ColorStop();

	return CAMERA_STATUS_SUCCESS;
}


int AstraD2C::GetCameraParam(OBCameraParams &cameraParam)
{

	int dataSize = sizeof(OBCameraParams);
	memset(&cameraParam, 0, sizeof(cameraParam));
	openni::Status rc = m_device.getProperty(openni::OBEXTENSION_ID_CAM_PARAMS, (uint8_t *)&cameraParam, &dataSize);
	if (rc != openni::STATUS_OK)
	{
		return CAMERA_STATUS_DEPTH_GET_CAMERA_PARAM_FAIL;
	}

	return CAMERA_STATUS_SUCCESS;
}

int AstraD2C::GetCameraResolution(int &nImageWidth, int &nImageHeight)
{
	uint16_t pid = m_device.getDeviceInfo().getUsbProductId();

	bool bFind = true;
	switch (pid)
	{
	case AstraPID_0401:
	case AstraPID_0402:
	case AstraPID_0407:
	case AstraPID_0408:
	{
		m_ImageWidth = IMAGE_WIDTH_640;
		m_ImageHeight = IMAGE_HEIGHT_480;
	}

	break;
	case  AstraPID_0404:
	{
		bool bRet = IsLunaDevice();
		if (bRet)
		{
			//Luna P1
			m_ImageWidth = IMAGE_WIDTH_640;
			m_ImageHeight = IMAGE_HEIGHT_400;
		}
		else
		{
			//Astra Mini
			m_ImageWidth = IMAGE_WIDTH_640;
			m_ImageHeight = IMAGE_HEIGHT_480;
		}

	}
	break;
	default:
		bFind = false;
		break;
	}

	if (bFind)
	{
		nImageWidth = m_ImageWidth;
		nImageHeight = m_ImageHeight;
		return CAMERA_STATUS_SUCCESS;
	}
	else
	{
		return CAMERA_STATUS_DEPTH_GET_RESOLUTION_FAIL;
	}
}

uint16_t AstraD2C::GetDevicePid(void)
{
	//
	uint16_t pid = m_device.getDeviceInfo().getUsbProductId();
	return pid;
}


int AstraD2C::ColorStart(int width, int height)
{
	if (m_bColorStart)
	{
		return CAMERA_STATUS_SUCCESS;
	}

	openni::Status rc = openni::STATUS_OK;
	if (NULL == m_device.getSensorInfo(SENSOR_COLOR))
	{
		return CAMERA_STATUS_DEPTH_CREATE_FAIL;
	}

	if (!m_bColorStreamCreate)
	{
		rc = m_ColorStream.create(m_device, SENSOR_COLOR);
		if (rc != STATUS_OK)
		{
			return CAMERA_STATUS_DEPTH_CREATE_FAIL;
		}

	}

	m_bColorStreamCreate = true;

	if (width > 0 && height > 0)
	{
		VideoMode mode = m_ColorStream.getVideoMode();
		mode.setResolution(width, height);
		m_ColorStream.setVideoMode(mode);
	}

	rc = m_ColorStream.start();
	if (rc != STATUS_OK)
	{
		return CAMERA_STATUS_DEPTH_START_FAIL;
	}

	m_bColorStart = true;

	printf("color start \n");

	return 0;
}

int AstraD2C::ColorStop()
{
	if (m_bColorStart)
	{
		m_ColorStream.stop();
	}

	m_bColorStart = false;

	if (m_bColorStreamCreate)
	{
		m_ColorStream.destroy();
	}

	m_bColorStreamCreate = false;
	printf("color stop \n");

	return CAMERA_STATUS_SUCCESS;
}

int AstraD2C::WaitColorStream(VideoFrameRef &frame)
{
	int changedIndex = -1;

	openni::Status rc = openni::STATUS_OK;

	openni::VideoStream* streams[] = { &m_ColorStream };

	rc = OpenNI::waitForAnyStream(streams, 1, &changedIndex, DEPTH_TIMEOUT);
	if (rc == openni::STATUS_OK)
	{
		rc = m_ColorStream.readFrame(&frame);
		if (rc != openni::STATUS_OK)
		{
			return CAMERA_STATUS_GET_RGB_FAIL;
		}
	}
	else
	{
		return CAMERA_STATUS_RGB_WAIT_TIMEOUT;
	}

	return CAMERA_STATUS_SUCCESS;
}

//字符数组中找字串
int FindString(char * pSrc, int srcSize, char * pDest, int dstSize)
{
	int iFind = -1;
	for (int i = 0; i<srcSize; i++){
		int iCnt = 0;
		for (int j = 0; j<dstSize; j++) {
			if (pDest[j] == pSrc[i + j])
				iCnt++;
		}
		if (iCnt == dstSize) {
			iFind = i;
			break;
		}
	}
	return iFind;
}

bool AstraD2C::IsLunaDevice(void)
{
	unsigned char strDeviceName[XN_DEVICE_MAX_STRING_LENGTH];
	int size = sizeof(strDeviceName);
	openni::Status rc = m_device.getProperty(XN_MODULE_PROPERTY_PHYSICAL_DEVICE_NAME, strDeviceName, &size);
	if (rc != openni::STATUS_OK)
	{
		printf("Error: %s\n", openni::OpenNI::getExtendedError());
		return false;
	}

	//
	string strLuna = "Luna";
	int nRet = FindString((char*)strDeviceName, XN_DEVICE_MAX_STRING_LENGTH, (char*)strLuna.c_str(), strLuna.size());
	if (nRet != -1)
	{
		return true;
	}

	return false;
}