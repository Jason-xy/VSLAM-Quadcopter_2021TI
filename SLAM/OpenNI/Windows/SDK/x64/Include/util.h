#ifndef __UTIL_H__
#define __UTIL_H__

#include <windows.h>
#include <string>
using std::wstring;
using std::string;

#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#define SAFE_DELETE(x) { if (x) delete x; x = NULL; }
#define SAFE_DELETE_ARRAY(x) { if (x) delete []x; x = NULL; }

typedef struct tagCameraInfo
{
	wchar_t deviceName[1024];
	wchar_t devicePath[1024];
	LONG dspRomSize;
	BYTE chipID;

	tagCameraInfo()
	{
		ZeroMemory(deviceName, sizeof(deviceName));
		ZeroMemory(devicePath, sizeof(devicePath));
		dspRomSize = 0;
		chipID = 0;
	}
}CameraInfo;

#endif