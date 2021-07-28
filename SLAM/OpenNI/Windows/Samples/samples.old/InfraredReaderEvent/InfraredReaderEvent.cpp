#include <stdio.h>
#include "OpenNI.h"

#include "OniSampleUtilities.h"

using namespace openni;

void analyzeFrame(const VideoFrameRef& frame)
{
	Grayscale16Pixel* pIR;

	int middleIndex = (frame.getHeight() + 1)*frame.getWidth() / 2;

	switch (frame.getVideoMode().getPixelFormat())
	{
	case PIXEL_FORMAT_GRAY16:
		pIR = (Grayscale16Pixel*)frame.getData();
		printf("[%08llu] %8d\n", (long long)frame.getTimestamp(),
			pIR[middleIndex]);
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

	VideoStream ir;

	//create depth stream
	if (device.getSensorInfo(SENSOR_IR) != NULL)
	{
		rc = ir.create(device, SENSOR_IR);
		if (rc != STATUS_OK)
		{
			printf("Couldn't create ir stream\n%s\n", OpenNI::getExtendedError());
		}
	}

	//start ir stream
	rc = ir.start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the ir stream\n%s\n", OpenNI::getExtendedError());
	}

	PrintCallback irPrinter;

	// Register frame listener
	ir.addNewFrameListener(&irPrinter);

	// Wait while we're getting frames through the printer
	while (!wasKeyboardHit())
	{
		Sleep(100);
	}

	ir.removeNewFrameListener(&irPrinter);

	//stop ir stream
	ir.stop();

	//destroy ir stream
	ir.destroy();

	//close device
	device.close();

	//shutdown OpenNI
	OpenNI::shutdown();

	return 0;
}