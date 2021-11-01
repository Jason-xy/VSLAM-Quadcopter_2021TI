/*****************************************************************************
*                                                                            *
*  OpenNI 2.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
#include <stdio.h>
#include "MultiCamera.h"
#include "util.h"
#include "SonixCamera.h"


int main()
{
	CameraInfo camerainfo[10];
	LONG cameranum = 0;
	BYTE serialNumberBuf[40];
	HRESULT hr = SonixCam_Init();
	if (hr == S_FALSE)
	{
		int nTimes = 20;
		while (nTimes >= 0)
		{
			hr = SonixCam_Init();
			nTimes--;
			if (hr == S_OK)
			{
				break;
			}
		}

	}
	if (hr == S_OK)
	{
		HRESULT rn = SonixCam_EnumDevice(camerainfo, cameranum, sizeof(camerainfo) / sizeof(CameraInfo));
		if (rn == S_OK)
		{
			for (int i = 0; i < cameranum; i++)
			{
				HRESULT sn = SonixCam_SelectDevice(i);
				if (sn == S_OK)
				{
					ZeroMemory(serialNumberBuf, sizeof(serialNumberBuf));
					int flag = SonixCam_GetSerialNumber(serialNumberBuf, sizeof(serialNumberBuf));
					if (flag != NOERROR)
					{
						continue;
					}
					printf("i=%d,SN=%s\n", i, serialNumberBuf);
					
				}
				else
				{
					continue;
				}
			}
		}

	}
	system("pause");
	return 0;
}



