#ifndef __MULTICAMERA_H__
#define __MULTICAMERA_H__
#include "util.h"
#include "SonixCamera.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ö���豸
//����SonixCam_EnumDevice���������Ҫ���������ӿں�������Ҫ�ȵ���SonixCam_SelectDeviceѡ��Ҫ�������豸��Ϊ����
//SonixCam_EnumDevice��Ĭ��ָ�����һ��ö�ٵ����豸��
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_EnumDevice
*	Description:		ö������Sonix�豸
*	Parameters:	pCameras��CameraInfo����ָ�룬deviceNum�����ػ�õ��豸���� maxDeviceNum��sizeof(pCameras) / sizeof(CameraInfo)��
*	Return :			�ɹ�����	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_EnumDevice(CameraInfo pCameras[], LONG &deviceNum, LONG maxDeviceNum);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ѡ���豸
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_SelectDevice
*	Description:		ѡ��ָ�����豸
*	Parameters:	devIndex��Ҫѡ�е��豸�����ţ���һ���豸������0,�������ֵ��deviceNum - 1��
*	Return :			�ɹ�����	S_OK
*/

extern "C"  HRESULT WINAPI SonixCam_SelectDevice(LONG devIndex);




////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���к���֧�ֶ��̲߳���
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
