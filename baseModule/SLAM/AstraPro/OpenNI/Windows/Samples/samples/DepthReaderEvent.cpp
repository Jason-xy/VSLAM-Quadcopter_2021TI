#include <stdio.h>
#include "OpenNI.h"

#include "OniSampleUtilities.h"

using namespace openni;

void analyzeFrame(const VideoFrameRef& frame)
{
	DepthPixel* pDepth;

	int middleIndex = (frame.getHeight() + 1)*frame.getWidth() / 2;

	switch (frame.getVideoMode().getPixelFormat())
	{
	case PIXEL_FORMAT_DEPTH_1_MM:
	case PIXEL_FORMAT_DEPTH_100_UM:
		pDepth = (DepthPixel*)frame.getData();
		printf("[%08llu] %8d\n", (long long)frame.getTimestamp(),
			pDepth[middleIndex]);
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

	VideoStream depth;

	//create depth stream
	if (device.getSensorInfo(SENSOR_DEPTH) != NULL)
	{
		rc = depth.create(device, SENSOR_DEPTH);
		if (rc != STATUS_OK)
		{
			printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
		}
	}

	//start depth stream
	rc = depth.start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
	}

	PrintCallback depthPrinter;

	// Register frame listener
	depth.addNewFrameListener(&depthPrinter);

	// Wait while we're getting frames through the printer
	while (!wasKeyboardHit())
	{
		Sleep(100);
	}

	depth.removeNewFrameListener(&depthPrinter);

	//stop depth stream
	depth.stop();

	//destroy depth stream
	depth.destroy();

	//close device
	device.close();

	//shutdown OpenNI
	OpenNI::shutdown();

	return 0;
}