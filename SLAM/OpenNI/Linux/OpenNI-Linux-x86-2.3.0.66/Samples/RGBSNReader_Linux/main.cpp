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
#include <stdlib.h>
#include <string.h>
#if defined(__cplusplus)
extern "C"
{
	#include "SonixCamera.h"
	#include "util.h"
}
#endif

#define vidpid  "2bc50532"    //Astra +  Camera's USB vidpid

int main()
{	
	if(TRUE != SonixCam_Init((char *)vidpid))
	{
		printf("Could not find/open sonix device, vidpid=%s\n", vidpid);
		exit(1);
	}
	printf("find uvc device\n");
	unsigned char data[100] = {0};
	int count=SonixCamera_num();
	for (int i = 0; i < count; i++)
	{
		 SonixCamera_SetDevicesID(i);
		memset(data, 0, sizeof(data));
		SonixCam_GetSerialNumber(data, sizeof(data));
		printf("SerialNumber: %s\n", (char*)data);
	}
	printf("close uvc sucess%d\n",SonixCam_UnInit());
	return 0;
}



