#ifndef __OB_UVCAPI_H__
#define __OB_UVCAPI_H__

#include <stdint.h>

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <string>

#include "OBTypes.h"
using namespace obuvc;

#define ONI_MAX_STR 256
#define ONI_MAX_SENSORS 10

struct ObGUID
{
	uint32_t data1;
	uint16_t data2, data3;
	uint8_t data4[8];
};

struct ObExtensionUnit
{
	int subdevice, unit, node;
	ObGUID id;
};


struct ObUVCDevice;

typedef struct{
	//ObSenserType sensorType;
	std::vector<ObVideoMode> supportedVideoModes;
}ObSensorInfo;

typedef struct{
	int size;
	ObSensorInfo sensorInfo[ONI_MAX_SENSORS];
}ObSensorInfos;


/** Basic description of a device */
typedef struct
{
	char uri[ONI_MAX_STR];
	char vendor[ONI_MAX_STR];
	char name[ONI_MAX_STR];
	uint16_t usbVendorId;
	uint16_t usbProductId;
} ObDeviceInfo;

int  ObUVCInit();
int ObUVCShutdown();

int EnumerateAllDevices(std::map<std::string, std::shared_ptr<ObUVCDevice>> &devices);
int EnumerateDevices(uint16_t nVendorID, uint16_t nProductID, std::map<std::string, std::shared_ptr<ObUVCDevice>> device, std::vector<std::string> &support_devices_uri);

bool IsDeviceConnected(ObUVCDevice & device, int vid, int pid);
int GetVendorId(const ObUVCDevice & device);
int GetProductId(const ObUVCDevice & device);
ObDeviceInfo * GetDeviceInfo(ObUVCDevice & device);

void SetStream(ObUVCDevice & device, int subdevice_index, void * pstream);

void SetControl(ObUVCDevice & device, const ObExtensionUnit & xu, uint8_t ctrl, void * data, int len);
void GetControl(const ObUVCDevice & device, const ObExtensionUnit & xu, uint8_t ctrl, void * data, int len);

// Control streaming
typedef std::function<void(const void * frame, int size, void *pstream)> video_channel_callback;

void SetSubdeviceMode(ObUVCDevice & device, int subdevice_index, int image_type, int width, int height, int fps, video_channel_callback callback);

//
void StartStreaming(ObUVCDevice & device,int subdevice_index=0);
void StopStreaming(ObUVCDevice & device, int subdevice_index=0);

void StopAllStreaming(ObUVCDevice & device);
void DestroyStreaming(ObUVCDevice & device, int subdevice_index = 0);

std::vector<ObVideoMode>EnumImageFormat(ObUVCDevice & device, int subdevice_index);

ObSensorInfos EnumImageFormat(ObUVCDevice & device);

#endif
