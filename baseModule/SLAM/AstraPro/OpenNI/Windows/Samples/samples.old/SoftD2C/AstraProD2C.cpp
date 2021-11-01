#include "AstraProD2C.h"

#include <string>
using namespace std;


AstroProD2C::AstroProD2C()
{
	m_pUvcBuff = new uint8_t[IMAGE_WIDTH_640*IMAGE_HEIGHT_480 * 2];
	memset(m_pUvcBuff, 0, IMAGE_WIDTH_640*IMAGE_HEIGHT_480 * 2);
	m_bUvcInit = false;
	m_bUvcStart = false;

	m_ImageWidth = IMAGE_WIDTH_640;
	m_ImageHeight = IMAGE_HEIGHT_480;

	m_bDepthInit = false;
	m_bDepthStart = false;
	m_bDepStreamCreate = false;

	m_bD2cType = HARDWARE_D2C;
}


AstroProD2C::~AstroProD2C()
{
	if (m_pUvcBuff)
	{
		delete[] m_pUvcBuff;
		m_pUvcBuff = NULL;
	}
}


int AstroProD2C::DepthInit(void)
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

int AstroProD2C::DepthUnInit()
{
	if (m_bDepthInit)
	{
		Depthstop();
		m_device.close();
		OpenNI::shutdown();
	}

	m_bDepthInit = false;

	printf("depth deinit \n");

	return CAMERA_STATUS_SUCCESS;
}

int AstroProD2C::Depthstart(int width, int height)
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
		//hardware d2c must set
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

int AstroProD2C::Depthstop()
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

int AstroProD2C::WaitDepthStream(VideoFrameRef &frame)
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


int AstroProD2C::CameraInit(int d2cType)
{
	int nRet = DepthInit();
	if (nRet == CAMERA_STATUS_SUCCESS)
	{
		//UVC INIT
		nRet = m_UvcSwapper.UvcInit(IMAGE_WIDTH_640, IMAGE_HEIGHT_480, OB_PIXEL_FORMAT_MJPEG); //OB_PIXEL_FORMAT_YUV422
		if (nRet != CAMERA_STATUS_SUCCESS)
		{
			return nRet;
		}

		m_bUvcInit = true;

		printf("depth init succ \n");
	}
	else
	{
		return nRet;
	}

	m_bD2cType = d2cType;
	return CAMERA_STATUS_SUCCESS;
}


int AstroProD2C::CameraUnInit(void)
{
	DepthUnInit();
	m_UvcSwapper.UvcDeInit();
	m_bUvcStart = false;
	m_bUvcInit = false;

	printf("CameraUnInit \n");
	return CAMERA_STATUS_SUCCESS;
}



int AstroProD2C::GetStreamData(cv::Mat &cv_rgb, cv::Mat &cv_depth)
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


	if (!m_bUvcInit)
	{
		int nRet = m_UvcSwapper.UvcInit(IMAGE_WIDTH_640, IMAGE_HEIGHT_480, OB_PIXEL_FORMAT_MJPEG);
		if (nRet != CAMERA_STATUS_SUCCESS)
		{
			return CAMERA_STATUS_RGB_INIT_FAIL;
		}
	}


	//¿ªÆôUVCÁ÷
	if (!m_bUvcStart)
	{
		m_UvcSwapper.UVCStreamStart();
		m_bUvcStart = true;

		printf("UVCStreamStart \n");
	}

	//DWORD nDepthStartTime = GetTickCount();
	DWORD beforeDepthRunTime = GetTickCount() - nStartTime;
	//printf("beforeDepthRunTime:%d\n", beforeDepthRunTime);

	VideoFrameRef frame;
	nRet = WaitDepthStream(frame);
	if (nRet == CAMERA_STATUS_SUCCESS)
	{
		/*
		uint16_t pid = m_device.getDeviceInfo().getUsbProductId();
		if (ASTRA_D2_PID == pid)
		{
			if (frame.getWidth() == m_ImageHeight && frame.getHeight() == m_ImageWidth)
			{
				memcpy(cv_depth.data, frame.getData(), sizeof(OniDepthPixel) * frame.getWidth() * frame.getHeight());
				cv_depth.cols = frame.getWidth();
				cv_depth.rows = frame.getHeight();
			}
		}
		else
		{
			if (frame.getWidth() == m_ImageWidth && frame.getHeight() == m_ImageHeight)
			{
				int nFrameSize = sizeof(OniDepthPixel) * frame.getWidth() * frame.getHeight();
				memcpy(cv_depth.data, frame.getData(), nFrameSize);
				cv_depth.cols = frame.getWidth();
				cv_depth.rows = frame.getHeight();

			}
		}
		*/

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

	DWORD depthRunTime = GetTickCount() - nStartTime;
	//printf("depth run time :%d\n", depthRunTime);
	uint32_t nSize = 0;
	uint32_t nImageType = 0;
	nRet = m_UvcSwapper.WaitUvcStream(m_pUvcBuff, nSize, nImageType, UVC_TIME_OUT);
	if (nRet != CAMERA_STATUS_SUCCESS)
	{
		return nRet;
	}

	DWORD uvcRunTime = GetTickCount() - nStartTime;
	//printf("uvc run time :%d\n", uvcRunTime);

	//MJPEG ×ª RGB888
	if (nImageType == OB_PIXEL_FORMAT_MJPEG)
	{
		//opencv mjpeg to rgb888
		cv::Mat img_decode;
		vector<uchar> data;
		for (int i = 0; i < nSize; ++i) {
			data.push_back(m_pUvcBuff[i]);
		}

		img_decode = cv::imdecode(data, CV_LOAD_IMAGE_COLOR);

		cv_rgb = img_decode.clone();

		DWORD decodeRunTime = GetTickCount() - nStartTime;
		//printf("uvc decode :%d\n", decodeRunTime);
		if (cv_rgb.cols == 0 || cv_rgb.rows == 0)
		{
			printf("mjpeg decode fail\n");
			return CAMERA_STATUS_GET_RGB_FAIL;
		}
	}
	//else if (nImageType == OB_PIXEL_FORMAT_YUV422)
	//{
	//	//todo:YUV422(YUYV)

	//}
	else
	{
		return CAMERA_STATUS_GET_RGB_FAIL;
	}

	return CAMERA_STATUS_SUCCESS;
}

int AstroProD2C::StreamStop(void)
{
	Depthstop();
	m_UvcSwapper.UVCStreamStop();

	m_bUvcStart = false;

	return CAMERA_STATUS_SUCCESS;
}


int AstroProD2C::GetCameraParam(OBCameraParams &cameraParam)
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

int AstroProD2C::GetCameraResolution(int &nImageWidth, int &nImageHeight)
{
	uint16_t pid = m_device.getDeviceInfo().getUsbProductId();

	bool bFind =true;
	switch (pid)
	{
	//case ASTRA_D2_PID:
	case ATLAS_PID:
	{
		m_ImageWidth = IMAGE_WIDTH_400;
		m_ImageHeight = IMAGE_HEIGHT_640;
	}
	break;
	case ASTRAPRO_PID:
	case ButterFly_PID:
	case ASTRAPLUS_PID:
	case AstraProPlus_PID:
	{
		m_ImageWidth = IMAGE_WIDTH_640;
		m_ImageHeight = IMAGE_HEIGHT_480;
	}

	break;
	case DEEYEA_PID:
	case Canglong_PID:
	case LunaP2_PID:
	case DaiBai_PID:
	case GEMINI_PID:
	case Projector_PID:
	{
		m_ImageWidth = IMAGE_WIDTH_640;
		m_ImageHeight = IMAGE_HEIGHT_400;
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

uint16_t AstroProD2C::GetDevicePid(void)
{
	//
	uint16_t pid = m_device.getDeviceInfo().getUsbProductId();
	return pid;
}