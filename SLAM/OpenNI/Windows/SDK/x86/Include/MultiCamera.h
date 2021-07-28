#ifndef __MULTICAMERA_H__
#define __MULTICAMERA_H__
#include "util.h"
#include "SonixCamera.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	枚举设备
//调用SonixCam_EnumDevice函数后如果要调用其它接口函数，需要先调用SonixCam_SelectDevice选中要操作的设备因为调用
//SonixCam_EnumDevice后默认指定最后一个枚举到的设备。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_EnumDevice
*	Description:		枚举所有Sonix设备
*	Parameters:	pCameras：CameraInfo数组指针，deviceNum：返回获得的设备数， maxDeviceNum：sizeof(pCameras) / sizeof(CameraInfo)。
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_EnumDevice(CameraInfo pCameras[], LONG &deviceNum, LONG maxDeviceNum);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	选中设备
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_SelectDevice
*	Description:		选中指定的设备
*	Parameters:	devIndex：要选中的设备索引号，第一个设备索引是0,最大索引值是deviceNum - 1。
*	Return :			成功返回	S_OK
*/

extern "C"  HRESULT WINAPI SonixCam_SelectDevice(LONG devIndex);




////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	下列函数支持多线程操作
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


extern "C"  HRESULT WINAPI SonixCam_MulGetSerialNumber(unsigned long devIndex, unsigned char pData[], long len);

extern "C"  HRESULT WINAPI SonixCam_MulGetVidPid(unsigned long devIndex, unsigned char pData[], long len);

extern "C"  HRESULT WINAPI SonixCam_MulSerialFlashCustomRead(unsigned long devIndex, long addr, unsigned char pData[], long len);

extern "C"  HRESULT WINAPI SonixCam_MulSerialFlashSectorCustomWrite(unsigned long devIndex, long addr, unsigned char pData[], long len, SERIAL_FLASH_TYPE sft);

extern "C"  HRESULT WINAPI SonixCam_MulSerialFlashRead(unsigned long devIndex, long addr, unsigned char pData[], long len);

extern "C"  HRESULT WINAPI SonixCam_MulSerialFlashSectorWrite(unsigned long devIndex, long addr, unsigned char pData[], long len, SERIAL_FLASH_TYPE sft);

extern "C"  HRESULT WINAPI SonixCam_MulSensorRegisterCustomRead(unsigned long devIndex, unsigned char slaveId, unsigned short  addr, unsigned char pData[], long len, bool pollSCL = false);

extern "C"  HRESULT WINAPI SonixCam_MulSensorRegisterCustomWrite(unsigned long devIndex, unsigned char slaveId, unsigned short addr, unsigned char pData[], long len, bool pollSCL = false);

extern "C"  HRESULT WINAPI SonixCam_MulAsicRegisterWrite(unsigned long devIndex, unsigned short addr, unsigned char pData[], long len);

extern "C"  HRESULT WINAPI SonixCam_MulAsicRegisterRead(unsigned long devIndex, unsigned short addr, unsigned char pData[], long len);

extern "C"  HRESULT WINAPI SonixCam_MulGetSerialFlashType(unsigned long devIndex, SERIAL_FLASH_TYPE &sft, bool check = false);

extern "C"  HRESULT WINAPI SonixCam_MulCustomBurnerFW(unsigned long devIndex, const ChangeParamInfo &paramInfo, unsigned char pFwBuffer[], LONG lFwLength, SonixCam_SetProgress setProgress, void *ptrClass, SERIAL_FLASH_TYPE sft, BOOL bFullCheckFW = FALSE);

extern "C"  HRESULT WINAPI SonixCam_MulSerialFlashWrite(unsigned long devIndex, long addr, unsigned char pData[], long len);

extern "C"  HRESULT WINAPI SonixCam_MulDisableSerialFlashWriteProtect(unsigned long devIndex, SERIAL_FLASH_TYPE sft);

extern "C"  HRESULT WINAPI SonixCam_MulEraseSerialFlash(unsigned long devIndex, SERIAL_FLASH_TYPE sft);

extern "C"  HRESULT WINAPI SonixCam_MulEraseSectorFlash(unsigned long devIndex, long addr, SERIAL_FLASH_TYPE sft);

extern "C"  HRESULT WINAPI SonixCam_MulEraseBlockFlash(unsigned long devIndex, long addr, SERIAL_FLASH_TYPE sft);

extern "C"  HRESULT WINAPI SonixCam_MulExportFW(unsigned long devIndex, unsigned char  pFwBuffer[], LONG lFwLength, SonixCam_SetProgress setProgress, void *ptrClass);

extern "C"  HRESULT WINAPI SonixCam_MulGetFWVersion(unsigned long devIndex, unsigned char pData[], long len, BOOL bNormalExport = TRUE);


#endif
