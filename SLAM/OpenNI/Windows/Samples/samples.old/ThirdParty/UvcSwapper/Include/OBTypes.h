#ifndef __ORBBEC_TYPES_H_
#define __ORBBEC_TYPES_H_

#define ORBBEC_ARR 256
#include <string>
#include <vector>
#include <memory>

namespace obuvc{

	typedef enum ObSenserType{
		//OB_DEPTH = 0,
		OB_IR = 0,
		OB_COLOR = 1,
		OB_TYPE_END = 2
	}ObSenserType;

	typedef struct{
		uint16_t  vid;
		uint16_t  pid;
		uint16_t bInterfaceNumber;
		std::string path;

	}SubDevice;

	typedef enum
	{
		// ir
		OB_PIXEL_FORMAT_RAW10 = 1,

		//depth
		OB_PIXEL_FORMAT_RAW11 = 2,
		ON_PIXEL_FORMAT_RAW12 = 3,

		//color
		OB_PIXEL_FORMAT_YUV422 = 4,
		OB_PIXEL_FORMAT_MJPEG = 5,
		OB_PIXEL_FORMAT_NV12 =6,


		OB_PIXEL_FORMAT_UNKNOWN = -1,
	} ObPixelFormat;


	/** Description of the output: format and resolution */
	typedef struct
	{
		ObPixelFormat pixelFormat;
		int resolutionX;
		int resolutionY;
		int fps;
	} ObVideoMode;


}
#endif
