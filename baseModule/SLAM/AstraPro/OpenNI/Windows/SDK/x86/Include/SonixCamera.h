#ifndef __SONIXCAMERA_H__
#define __SONIXCAMERA_H__


typedef enum
{
	EC_UnKnow,
	EC_LibUnInit,
	EC_EnumDeviceFail,
	EC_DisableFlashWriteProtectFail,
	EC_EraseFlashFail,
	EC_EraseFlashSectorFail,
	EC_GetAsicIdFail,
	EC_GetAsicRomVersionFail,
	EC_GetAsicRomTypeFail,
	EC_ReadAsicRegisterFail,
	EC_WriteAsicRegisterFail,
	EC_UnKnowSerialFlashType,
	EC_BurnerCheckFail,
	EC_CoInitializeFail,
	EC_NoFindDevice,
	EC_MallocMemoryFail,
}ERROR_CODE;

typedef enum
{
	SFT_UNKNOW,
	SFT_MXIC,
	SFT_ST,
	SFT_SST,
	SFT_ATMEL_AT25F,
	SFT_ATMEL_AT25FS,
	SFT_ATMEL_AT45DB,
	SFT_WINBOND,
	SFT_PMC,
	SFT_MXIC_LIKE,
	SFT_AMIC,
	SFT_EON,
	SFT_ESMT,
	SFT_GIGA,
	SFT_FENTECH
}SERIAL_FLASH_TYPE;

typedef enum
{
	ART_UNKNOW = -1,
	ART_32K,
	ART_64K,
	ART_128K,
	ART_256K
}ASIC_ROM_TYPE;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	库初始化
//在调用库的接口函数之前，需要首先调用SonixCam_Init函数，只有该函数返回成功后，才可以调用其它接口函数
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_Init
*	Description:		库的初始化
*	Parameters:		vidpid：如果指定vidpid，则会寻找并指向设备，如果没有找到，返回S_FALSE。
*					不指定vidpid，则找到Sonix设备后直接返回S_OK，并指向设备，没有找到，返回S_FALSE
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_Init(char *vidpid = NULL);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_UnInit
*	Description:		
*	Parameters:	
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_UnInit();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	重启设备
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_RestartDevice
*	Description:		
*	Parameters:
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_RestartDevice();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得错误代码
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetErrorCode
*	Description:
*	Parameters:
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetErrorCode(ERROR_CODE &ec);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得当前设备的BaseFilter指针
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetCurBaseFilter
*	Description:		
*	Parameters:		ppBaseFilter:成功返回设备的pBaseFilter指针
*	Return :			成功返回	S_OK
*/
extern "C" HRESULT WINAPI SonixCam_GetCurBaseFilter(void **ppBaseFilter);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Dsp寄存器读
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_AsicRegisterRead
*	Description:
*	Parameters:		addr:dsp寄存器地址，pData读取数据缓存，len：要读取的数据字节数。
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_AsicRegisterRead(unsigned short addr, unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Dsp寄存器写
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_AsicRegisterWrite
*	Description:
*	Parameters:		addr:dsp寄存器地址，pData：写数据缓存，len：要写的数据字节数。
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_AsicRegisterWrite(unsigned short addr, unsigned char pData[], long len);

//get nodeId
extern "C"  HRESULT WINAPI SonixCam_GetNodeId(LONG &nodeId, LONG id);
// XU read
extern "C"  HRESULT WINAPI SonixCam_XuRead(unsigned char pData[], unsigned int length, BYTE cs, LONG nodeId);
// XU write
extern "C"  HRESULT WINAPI SonixCam_XuWrite(unsigned char pData[], unsigned int length, BYTE cs, LONG nodeId);

extern "C"  HRESULT WINAPI SonixCam_SensorRegisterRead(unsigned char slaveId, unsigned char  addr, unsigned char pData[], long len);

extern "C"  HRESULT WINAPI SonixCam_SensorRegisterWrite(unsigned char slaveId, unsigned char  addr, unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	I2C读
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_SensorRegisterCustomRead
*	Description:
*	Parameters:		slaveId:I2c子设备ID，addr：要读的地址，pData：读缓存， len：读数据长度， pollSCL：SCL 拉高
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_SensorRegisterCustomRead(unsigned char slaveId, unsigned short  addr, unsigned char pData[], long len, bool pollSCL = false);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	I2C写
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_SensorRegisterCustomWrite
*	Description:
*	Parameters:		slaveId:I2c子设备ID，addr：要写的地址，pData：写缓存， len：写数据长度， pollSCL：SCL 拉高
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_SensorRegisterCustomWrite(unsigned char slaveId, unsigned short addr, unsigned char pData[], long len, bool pollSCL = false);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	FLASH读，读地址最大范围是0-256k空间
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_SerialFlashRead
*	Description:
*	Parameters:		addr：要写的地址，pData：写缓存， len：写数据长度
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_SerialFlashRead(long addr, unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	FLASH读，读速度比SonixCam_SerialFlashRead慢，但是读地址空间更大
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_SerialFlashRead
*	Description:
*	Parameters:		addr：要写的地址，pData：写缓存， len：写数据长度
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_SerialFlashCustomRead(long addr, unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	FLASH写，写地址最大范围是0-256k空间，不要在开图的情况下调用该函数。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_SerialFlashSectorWrite
*	Description:		先关闭FLASH的写保护，并擦除参数addr地址所在的Flash的扇区，然后写入数据。
*	Parameters:		addr：写的地址，pData：写缓存， len：写数据长度，sft: FLASH的类型
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_SerialFlashSectorWrite(long addr, unsigned char pData[], long len, SERIAL_FLASH_TYPE sft);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	FLASH写，写地址最大范围是0-128k空间，不要在开图的情况下调用该函数。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_SerialFlashWrite
*	Description:		与SonixCam_SerialFlashSectorWrite的区别是它不擦除addr地址所在的扇区，需要确保调用该函数之前
*					已经关闭Flash的写保护并且addr地址要写的区域已经擦除。
*	Parameters:		addr：写的地址，pData：写缓存， len：写数据长度
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_SerialFlashWrite(long addr, unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	FLASH写，写速度比SonixCam_SerialFlashSectorWrite慢，但是写地址空间更大，不要在开图的情况下调用该函数。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_SerialFlashSectorCustomWrite
*	Description:
*	Parameters:		addr：写的地址，pData：写缓存， len：写数据长度，sft: FLASH的类型
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_SerialFlashSectorCustomWrite(long addr, unsigned char pData[], long len, SERIAL_FLASH_TYPE sft);

extern "C"  HRESULT WINAPI SonixCam_SerialFlashSectorWriteEx(long addr, unsigned char pData[], long len, SERIAL_FLASH_TYPE sft);

//get device information	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得FLASH的类型
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetSerialFlashType
*	Description:
*	Parameters:		sft：成功返回flash的类型，check：设为true，在flash读取flash类型
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetSerialFlashType(SERIAL_FLASH_TYPE &sft, bool check = false);

extern "C"  HRESULT WINAPI SonixCam_GetParamTableAddr(long &paramTableAddr, long &paramTableLength, long &crcAddr, unsigned char *pFW = NULL);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得FW Code版本号
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetFWVersion
*	Description:
*	Parameters:		pData：FW Code版本号缓存数组指针，len：缓存数组最大字节数，bNormalExport：为TRUE以默认方式排列版本号字符串，为FALSH，以第二种方式排列版本号字符串, 和原始固件名相同。
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetFWVersion(unsigned char pData[], long len, BOOL bNormalExport = TRUE);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	从固件文件中获得FW Code版本号
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetFWVersionFromFile
*	Description:
*	Parameters:		pData：FW Code版本号缓存数组指针，len：缓存数组最大字节数，bNormalExport：为TRUE以默认方式排列版本号字符串，为FALSH，以第二种方式排列版本号字符串，和原始固件名相同。
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetFWVersionFromFile(unsigned char pFwFile[], unsigned char pData[], long len, BOOL bNormalExport = TRUE);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得设备Manufacturer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetManufacturer
*	Description:
*	Parameters:		pData：Manufacturer缓存数组指针，len：缓存数组最大字节数
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetManufacturer(unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得设备Product
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetProduct
*	Description:
*	Parameters:		pData：Product缓存数组指针，len：缓存数组最大字节数
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetProduct(unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得设备vidpid
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetVidPid
*	Description:
*	Parameters:		pData：vidpid缓存数组指针，len：缓存数组最大字节数
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetVidPid(unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得设备String3
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetString3
*	Description:
*	Parameters:		pData：String3缓存数组指针，len：缓存数组最大字节数
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetString3(unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得设备Interface
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetInterface
*	Description:
*	Parameters:		pData：Interface缓存数组指针，len：缓存数组最大字节数
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetInterface(unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得设备序列号
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:		SonixCam_GetSerialNumber
*	Description:
*	Parameters:		pData：序列号缓存数组指针，len：缓存数组最大字节数
*	Return :			成功返回	S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetSerialNumber(unsigned char pData[], long len);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	进度回调函数指针
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Description:	SonixCam_BurnerFW， SonixCam_WriteFwToFlash， SonixCam_ExportFW，SonixCam_FullCheckFW， SonixCam_CustomBurnerFW，
*				SonixCam_SetParamTableFromFWFile中可以传递函数指针来更新烧录进度
*	Parameters:	ptrClass：类指针，fProcess：当前进度值
*	Return :		成功返回S_OK
*/
typedef void(*SonixCam_SetProgress)(void *ptrClass, float fProcess);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	获得rom类型
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_GetAsicRomType
*	Description:	
*	Parameters:	romType：返回rom类型，chipID：当前dsp的芯片id值
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_GetAsicRomType(ASIC_ROM_TYPE &romType, unsigned char &chipID);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	烧录固件，不要在开图的情况下调用该函数。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_BurnerFW
*	Description: 首先会关闭Flash的写保护，然后擦除整个Flash区域，然后将固件写入到Flash
*	Parameters:	pFwBuffer：要烧录的固件缓存指针，lFwLength：固件缓存长度， setProgress：烧录进度回调函数指针， ptrClass：当前类， sft：flash类型， bFullCheckFW：烧录完成后是否进行全校验
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_BurnerFW(unsigned char pFwBuffer[], LONG lFwLength, SonixCam_SetProgress setProgress, void *ptrClass, SERIAL_FLASH_TYPE sft, BOOL bFullCheckFW = FALSE);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	烧录固件，不要在开图的情况下调用该函数。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_WriteFwToFlash
*	Description: 该函数与SonixCam_BurnerFW的区别是它不会调用关闭写保护和擦除Flash，需要在调用该函数之前自己关闭Flash写保护和擦除Flash相应区域
*	Parameters:	pFwBuffer：要烧录的固件缓存指针，lFwLength：固件缓存长度， setProgress：烧录进度回调函数指针， ptrClass：当前类， bFullCheckFW：烧录完成后是否进行全校验
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_WriteFwToFlash(unsigned char pFwBuffer[], LONG lFwLength, SonixCam_SetProgress setProgress, void *ptrClass, BOOL bFullCheckFW = FALSE);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	导出固件
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_ExportFW
*	Description:
*	Parameters:	pFwBuffer：要保存固件缓存指针，lFwLength：固件缓存长度， setProgress：导出进度回调函数指针， ptrClass：当前类
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_ExportFW(unsigned char  pFwBuffer[], LONG lFwLength, SonixCam_SetProgress setProgress, void *ptrClass);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	关闭FLASH的写保护
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_DisableSerialFlashWriteProtect
*	Description:
*	Parameters:	sft：FLASH类型
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_DisableSerialFlashWriteProtect(SERIAL_FLASH_TYPE sft);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	擦除FLASH
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_EraseSerialFlash
*	Description:
*	Parameters:	sft：FLASH类型
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_EraseSerialFlash(SERIAL_FLASH_TYPE sft);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	擦除FLASH的一个扇区
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_EraseSectorFlash
*	Description:
*	Parameters:	addr：要擦除的扇区地址，sft：FLASH类型
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_EraseSectorFlash(long addr, SERIAL_FLASH_TYPE sft);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	擦除FLASH的一个块区
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_EraseBlockFlash
*	Description:
*	Parameters:	addr：要擦除的块区地址，sft：FLASH类型
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_EraseBlockFlash(long addr, SERIAL_FLASH_TYPE sft);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	全校验FW
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_FullCheckFW
*	Description: 将pFwBuffer里面的固件和Flash中的固件进行比较是否相同，相同返回S_OK
*	Parameters:	pFwBuffer：要进行比较的固件缓存指针，fwLength：缓存长度，setProgress：比较进度回调函数指针， ptrClass：当前类 
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_FullCheckFW(unsigned char pFwBuffer[], LONG fwLength, SonixCam_SetProgress setProgress, void *ptrClass);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	参数表信息
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	将需要修改的参数字符串赋值给相应的指针，并填写相应的参数字符串字符长度
*/

typedef struct tagChangeParamInfo
{
	unsigned int SerialNumberLength;
	unsigned int ProductLength;
	unsigned int ManufactureLength;
	unsigned int VidPidLength;
	unsigned int InterfaceLength;
	unsigned int String3Length;
	char* pSerialNumber;
	char* pProduct;
	char* pManufacture;
	char* pVidPid;
	char* pString3;
	char* pInterface;

	tagChangeParamInfo()
	{
		SerialNumberLength = 0;
		ProductLength = 0;
		ManufactureLength = 0;
		VidPidLength = 0;
		InterfaceLength = 0;
		String3Length = 0;
		pSerialNumber = 0;
		pProduct = 0;
		pManufacture = 0;
		pVidPid = 0;
		pString3 = 0;
		pInterface = 0;
	}
}ChangeParamInfo;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	自定义烧录固件，不要在开图的情况下调用该函数。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_CustomBurnerFW
*	Description:	该函数和SonixCam_BurnerFW的区别是它可以更改设备的一些参数
*	Parameters:	paramInfo：要更改的参数信息，pFwBuffer：固件缓存指针，lFwLength：缓存长度，setProgress：烧录进度回调函数指针， ptrClass：当前类， sft:FLASH类型，bFullCheckFW：是否烧录完成后进行全校验
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_CustomBurnerFW(const ChangeParamInfo &paramInfo, unsigned char pFwBuffer[], LONG lFwLength, SonixCam_SetProgress setProgress, void *ptrClass, SERIAL_FLASH_TYPE sft, BOOL bFullCheckFW = FALSE);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	修改设备参数表信息，不要在开图的情况下调用该函数。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*	Function:	SonixCam_SetParamTableFromFWFile
*	Description:	该函数和SonixCam_BurnerFW的区别是它不需要把整个FW烧录到Flash，只需要烧录参数表所在的扇区数据，pFW固件缓存内容必须和设备FLASH中的固件相同
*	Parameters:	pFW：固件缓存指针，lFwLength：缓存长度，setProgress：烧录进度回调函数指针， ptrClass：当前类， sft:FLASH类型，pLogFilePath: 打印的Log文件路径，可以为空，则不打印Log信息， bFullCheckFW：是否烧录完成后进行全校验
*	Return :		成功返回S_OK
*/
extern "C"  HRESULT WINAPI SonixCam_SetParamTableFromFWFile(unsigned char pFW[], long lFwLength, const ChangeParamInfo &paramInfo, SonixCam_SetProgress setProgress, void *ptrClass, SERIAL_FLASH_TYPE sft, char* pLogFilePath, BOOL bFullCheckFW = FALSE);

extern "C"  HRESULT WINAPI SonixCam_SetParamTableFromReadFlashSector(const ChangeParamInfo &paramInfo, SonixCam_SetProgress setProgress, void *ptrClass, SERIAL_FLASH_TYPE sft, char* pLogFilePath, BOOL bFullCheckFW = FALSE);


#endif __SONIXCAMERA_H__