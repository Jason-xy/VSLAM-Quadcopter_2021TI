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
	PROPERTY_Brightness = 0,								// 亮度
	PROPERTY_Contrast = 1,								// 对比度
	PROPERTY_Hue = 2,										// 色调
	PROPERTY_Saturation = 3,								// 饱和度
	PROPERTY_Sharpness = 4,								// 清晰度
	PROPERTY_Gamma = 5,									// 伽马
	PROPERTY_ColorEnable = 6,								// 黑白
	PROPERTY_WhiteBalance = 7,								// 白平衡
	PROPERTY_BacklightCompensation = 8,					// 逆光对比
	PROPERTY_Gain = 9										// 增益
}CameraProperty;

typedef enum
{
	CONTROL_Pan = 0,											// 全景
	CONTROL_Tilt = 1,											// 倾斜
	CONTROL_Roll = 2,											// 镜像翻转
	CONTROL_Zoom = 3,											// 缩放
	CONTROL_Exposure = 4,										// 曝光
	CONTROL_Iris = 5,											// 光圈
	CONTROL_Focus = 6											// 焦点
} CameraControl;

typedef enum
{
	PROPERTY_FLAGS_Auto = 1,								// 自动
	PROPERTY_FLAGS_Manual = 2								// 手动
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
//	预览
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetPreviewResolutionCount
*	Description:		获取Camera支持的预览格式数量
*	Parameters:	    格式数量
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_GetPreviewResolutionCount(long* ResolutionCount);

/*
*	Function:		SonixCam_GetPreviewResolutionInfo
*	Description:		获取相应预览格式信息
*	Parameters:	格式编号，色彩空间，宽，高
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_GetPreviewResolutionInfo(unsigned char ResolutionNum, VideoOutFormat *OutFormat);


/*
*	Function:		SonixCam_GetCurPreviewResolutionInfo
*	Description:		获取预览格式信息
*	Parameters:	色彩空间，宽，高
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_GetCurrentPreviewResolutionInfo(VideoOutFormat *OutFormat);


/*
*	Function:		SonixCam_SetVideoFormat
*	Description:		设置预览格式
*	Parameters:	    格式编号
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_SetVideoFormat(long lNotifyWinMsg, unsigned char ResolutionNum);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	拍照
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetStillResolutionCount
*	Description:		获取Camera支持的拍照格式数量
*	Parameters:	    格式数量
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_GetStillResolutionCount(long* ResolutionCount);

/*
*	Function:		SonixCam_GetStillResolutionInfo
*	Description:		获取相应拍照格式信息
*	Parameters:	    格式编号，颜色空间，宽，高
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_GetStillResolutionInfo(unsigned char ResolutionNum, VideoOutFormat* OutFormat);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	属性及控制
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_PropertyGetRange
*	Description:		获取属性页项目范围、步进、默认值、是否自动
*	Parameters:	    属性值，最小值，最大值，步进，是否自动
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_PropertyGetRange(CameraProperty uiProperty,
	long* pMin, long* pMax,
	long* pSteppingDelta,
	long* pDefault,
	PropertyFlags* pCapsFlags
	);

/*
*	Function:		SonixCam_PropertyGet
*	Description:		获取属性页项目设置值
*	Parameters:	    属性值，设置值，是否自动
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_PropertyGet(CameraProperty uiProperty, long* lValue, PropertyFlags* Flags);

/*
*	Function:		SonixCam_PropertySet
*	Description:		设置属性页项目
*	Parameters:	    属性值，设置值，是否自动
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_PropertySet(CameraProperty uiProperty, long lValue, PropertyFlags Flags);

/*
*	Function:		SonixCam_ControlGetRange
*	Description:		获取控制页项目范围、步进、默认值、是否自动
*	Parameters:	    控制值，最小值，最大值，步进，是否自动
*	Return :			返回状态
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
*	Description:		获取控制页项目设置值
*	Parameters:	    控制值，设置值，是否自动
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_ControlGet(CameraControl uiControl, long* lValue, PropertyFlags* Flags);

/*
*	Function:		SonixCam_ControlSet
*	Description:		设置控制页项目
*	Parameters:	    控制值，设置值，是否自动
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_ControlSet(CameraControl uiControl,long lValue,PropertyFlags Flags);

/*
*	Function:		WINAPISonixCam_GetFrameSpeed
*	Description:		获取当前帧率
*	Parameters:	    帧率
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_GetFrameSpeed(double* dFramerate);

/*
*	Function:		SonixCam_SetFrameSpeed
*	Description:		设置帧率
*	Parameters:	    帧率
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_SetFrameSpeed(double dFramerate);

/*
*	Function:		SonixCam_SetPowerLine
*	Description:		设置电源频率
*	Parameters:	    电源频率
*	Return :			返回状态
*/
extern "C"  HRESULT WINAPI SonixCam_SetPowerLine(PowerLine PowerLine);



#endif



