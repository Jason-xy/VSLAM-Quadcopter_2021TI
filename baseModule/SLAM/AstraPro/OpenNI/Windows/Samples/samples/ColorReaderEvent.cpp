#include <stdio.h>
#include "OpenNI.h"

#include "OniSampleUtilities.h"

using namespace openni;

void analyzeFrame(const VideoFrameRef& frame)
{
	RGB888Pixel* pColor;

	int middleIndex = (frame.getHeight() + 1)*frame.getWidth() / 2;

	switch (frame.getVideoMode().getPixelFormat())
	{
	case PIXEL_FORMAT_RGB888:
		pColor = (RGB888Pixel*)frame.getData();
		printf("[%08llu] 0x%02x%02x%02x\n", (long long)frame.getTimestamp(),
			pColor[middleIndex].r & 0xff,
			pColor[middleIndex].g & 0xff,
			pColor[middleIndex].b & 0xff);
		break;
	default:
		printf("Unknown format\n");
	}
}

class PrintCallback : public VideoStream::NewFrameListener
{
public:
	void onNewFrame(VideoStream& stream)
	{
		stream.readFrame(&m_frame);

		analyzeFrame(m_frame);
	}
private:
	VideoFrameRef m_frame;
};



int main(int argc, char* argv[])
{
	//initialize openNI sdk
	Status rc = OpenNI::initialize();
	if (rc != STATUS_OK)
	{
		printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
		return 1;
	}

	//open deivce
	Device device;
	rc = device.open(ANY_DEVICE);
	if (rc != STATUS_OK)
	{
		printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());
		return 2;
	}

	VideoStream color;

	//create color stream
	if (device.getSensorInfo(SENSOR_COLOR) != NULL)
	{
		rc = color.create(device, SENSOR_COLOR);
		if (rc != STATUS_OK)
		{
			printf("Couldn't create color stream\n%s\n", OpenNI::getExtendedError());
		}
	}

	//start color stream
	rc = color.start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the color stream\n%s\n", OpenNI::getExtendedError());
	}

	PrintCallback colorPrinter;

	// Register frame listener
	color.addNewFrameListener(&colorPrinter);

	// Wait while we're getting frames through the printer
	while (!wasKeyboardHit())
	{
		Sleep(100);
	}

	color.removeNewFrameListener(&colorPrinter);

	//stop color stream
	color.stop();

	//destroy color stream
	color.destroy();

	//close device
	device.close();

	//shutdown OpenNI
	OpenNI::shutdown();

	return 0;
}