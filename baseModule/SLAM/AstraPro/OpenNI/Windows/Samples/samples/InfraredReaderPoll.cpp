#include <stdio.h>
#include <OpenNI.h>

#include "OniSampleUtilities.h"

#define SAMPLE_READ_WAIT_TIMEOUT 2000 //2000ms

using namespace openni;

int main(int argc, char* argv[])
{
	//initialize openni sdk
	Status rc = OpenNI::initialize();
	if (rc != STATUS_OK)
	{
		printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
		return 1;
	}

	Device device;

	//open device
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
			return 3;
		}
	}

	//start ir stream
	rc = ir.start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the ir stream\n%s\n", OpenNI::getExtendedError());
		return 4;
	}

	VideoFrameRef frame;

	while (!wasKeyboardHit())
	{
		int changedStreamDummy;
		VideoStream* pStream = &ir;

		//wait a new frame
		rc = OpenNI::waitForAnyStream(&pStream, 1, &changedStreamDummy, SAMPLE_READ_WAIT_TIMEOUT);
		if (rc != STATUS_OK)
		{
			printf("Wait failed! (timeout is %d ms)\n%s\n", SAMPLE_READ_WAIT_TIMEOUT, OpenNI::getExtendedError());
			continue;
		}

		//get depth frame
		rc = ir.readFrame(&frame);
		if (rc != STATUS_OK)
		{
			printf("Read failed!\n%s\n", OpenNI::getExtendedError());
			continue;
		}

		//check if the frame format is infrared frame format
		if (frame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_GRAY16)
		{
			printf("Unexpected frame format\n");
			continue;
		}

		Grayscale16Pixel* pIr = (Grayscale16Pixel*)frame.getData();

		int middleIndex = (frame.getHeight() + 1)*frame.getWidth() / 2;

		//print the gray value of the middle pixel of the frame
		printf("[%08llu] %8d\n", (long long)frame.getTimestamp(), pIr[middleIndex]);
	}

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