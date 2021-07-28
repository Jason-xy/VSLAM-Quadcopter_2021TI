#ifndef __AUDIOVIDEO_H__
#define __AUDIOVIDEO_H__

typedef enum
{
	VCS_Unknow,
	VCS_H264,
	VCS_Mjpg,
	VCS_Yuy2,
	VCS_Y8
}VideoColorSpace;

typedef struct tagVideoOutFormat
{
	int width;
	int height;
	VideoColorSpace vcs;
}VideoOutFormat;

typedef enum
{
	PROPERTY_Brightness = 0,								// ����
	PROPERTY_Contrast = 1,								// �Աȶ�
	PROPERTY_Hue = 2,										// ɫ��
	PROPERTY_Saturation = 3,								// ���Ͷ�
	PROPERTY_Sharpness = 4,								// ������
	PROPERTY_Gamma = 5,									// ٤��
	PROPERTY_ColorEnable = 6,								// �ڰ�
	PROPERTY_WhiteBalance = 7,								// ��ƽ��
	PROPERTY_BacklightCompensation = 8,					// ���Ա�
	PROPERTY_Gain = 9										// ����
}CameraProperty;

typedef enum
{
	CONTROL_Pan = 0,											// ȫ��
	CONTROL_Tilt = 1,											// ��б
	CONTROL_Roll = 2,											// ����ת
	CONTROL_Zoom = 3,											// ����
	CONTROL_Exposure = 4,										// �ع�
	CONTROL_Iris = 5,											// ��Ȧ
	CONTROL_Focus = 6											// ����
} CameraControl;

typedef enum
{
	PROPERTY_FLAGS_Auto = 1,								// �Զ�
	PROPERTY_FLAGS_Manual = 2								// �ֶ�
} PropertyFlags;

typedef enum
{
	POWER_LINE_50Hz = 1,
	POWER_LINE_60Hz = 2
}PowerLine;


typedef int(CALLBACK* SonixCam_Grab)(BYTE *pBuffer, LONG bufferSize);

extern "C"  HRESULT WINAPI SonixCam_OpenCamera(HWND hWnd, LONG lNotifyWinMsg, SonixCam_Grab pGrab);
extern "C"  HRESULT WINAPI SonixCam_CloseCamera();

extern "C"  HRESULT WINAPI SonixCam_IsOpenCamera(bool &bOpening);
extern "C"  HRESULT WINAPI SonixCam_IsPreviewing(bool &bPreviewing);

extern "C"  HRESULT WINAPI SonixCam_StartPreview();
extern "C"  HRESULT WINAPI SonixCam_PausePreview();
extern "C"  HRESULT WINAPI SonixCam_StopPreview();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Ԥ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetPreviewResolutionCount
*	Description:		��ȡCamera֧�ֵ�Ԥ����ʽ����
*	Parameters:	    ��ʽ����
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_GetPreviewResolutionCount(long* ResolutionCount);

/*
*	Function:		SonixCam_GetPreviewResolutionInfo
*	Description:		��ȡ��ӦԤ����ʽ��Ϣ
*	Parameters:	��ʽ��ţ�ɫ�ʿռ䣬����
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_GetPreviewResolutionInfo(unsigned char ResolutionNum, VideoOutFormat *OutFormat);


/*
*	Function:		SonixCam_GetCurPreviewResolutionInfo
*	Description:		��ȡԤ����ʽ��Ϣ
*	Parameters:	ɫ�ʿռ䣬����
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_GetCurrentPreviewResolutionInfo(VideoOutFormat *OutFormat);


/*
*	Function:		SonixCam_SetVideoFormat
*	Description:		����Ԥ����ʽ
*	Parameters:	    ��ʽ���
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_SetVideoFormat(long lNotifyWinMsg, unsigned char ResolutionNum);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetStillResolutionCount
*	Description:		��ȡCamera֧�ֵ����ո�ʽ����
*	Parameters:	    ��ʽ����
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_GetStillResolutionCount(long* ResolutionCount);

/*
*	Function:		SonixCam_GetStillResolutionInfo
*	Description:		��ȡ��Ӧ���ո�ʽ��Ϣ
*	Parameters:	    ��ʽ��ţ���ɫ�ռ䣬����
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_GetStillResolutionInfo(unsigned char ResolutionNum, VideoOutFormat* OutFormat);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���Լ�����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_PropertyGetRange
*	Description:		��ȡ����ҳ��Ŀ��Χ��������Ĭ��ֵ���Ƿ��Զ�
*	Parameters:	    ����ֵ����Сֵ�����ֵ���������Ƿ��Զ�
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_PropertyGetRange(CameraProperty uiProperty,
	long* pMin, long* pMax,
	long* pSteppingDelta,
	long* pDefault,
	PropertyFlags* pCapsFlags
	);

/*
*	Function:		SonixCam_PropertyGet
*	Description:		��ȡ����ҳ��Ŀ����ֵ
*	Parameters:	    ����ֵ������ֵ���Ƿ��Զ�
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_PropertyGet(CameraProperty uiProperty, long* lValue, PropertyFlags* Flags);

/*
*	Function:		SonixCam_PropertySet
*	Description:		��������ҳ��Ŀ
*	Parameters:	    ����ֵ������ֵ���Ƿ��Զ�
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_PropertySet(CameraProperty uiProperty, long lValue, PropertyFlags Flags);

/*
*	Function:		SonixCam_ControlGetRange
*	Description:		��ȡ����ҳ��Ŀ��Χ��������Ĭ��ֵ���Ƿ��Զ�
*	Parameters:	    ����ֵ����Сֵ�����ֵ���������Ƿ��Զ�
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_ControlGetRange(
	CameraControl uiControl,
	long* pMin, long* pMax,
	long* pSteppingDelta,
	long* pDefault,
	PropertyFlags* pCapsFlags
	);

/*
*	Function:		SonixCam_ControlGet
*	Description:		��ȡ����ҳ��Ŀ����ֵ
*	Parameters:	    ����ֵ������ֵ���Ƿ��Զ�
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_ControlGet(CameraControl uiControl, long* lValue, PropertyFlags* Flags);

/*
*	Function:		SonixCam_ControlSet
*	Description:		���ÿ���ҳ��Ŀ
*	Parameters:	    ����ֵ������ֵ���Ƿ��Զ�
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_ControlSet(CameraControl uiControl,long lValue,PropertyFlags Flags);

/*
*	Function:		WINAPISonixCam_GetFrameSpeed
*	Description:		��ȡ��ǰ֡��
*	Parameters:	    ֡��
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_GetFrameSpeed(double* dFramerate);

/*
*	Function:		SonixCam_SetFrameSpeed
*	Description:		����֡��
*	Parameters:	    ֡��
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_SetFrameSpeed(double dFramerate);

/*
*	Function:		SonixCam_SetPowerLine
*	Description:		���õ�ԴƵ��
*	Parameters:	    ��ԴƵ��
*	Return :			����״̬
*/
extern "C"  HRESULT WINAPI SonixCam_SetPowerLine(PowerLine PowerLine);



#endif



