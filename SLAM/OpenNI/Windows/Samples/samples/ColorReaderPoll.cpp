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

	VideoStream color;

	//create color stream
	if (device.getSensorInfo(SENSOR_COLOR) != NULL)
	{
		rc = color.create(device, SENSOR_COLOR);
		if (rc != STATUS_OK)
		{
			printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
			return 3;
		}
	}

	//start color stream
	rc = color.start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
		return 4;
	}

	VideoFrameRef frame;
	RGB888Pixel* pColor;

	while (!wasKeyboardHit())
	{
		int changedStreamDummy;
		VideoStream* pStream = &color;

		//wait a new frame
		rc = OpenNI::waitForAnyStream(&pStream, 1, &changedStreamDummy, SAMPLE_READ_WAIT_TIMEOUT);
		if (rc != STATUS_OK)
		{
			printf("Wait failed! (timeout is %d ms)\n%s\n", SAMPLE_READ_WAIT_TIMEOUT, OpenNI::getExtendedError());
			continue;
		}

		//get color frame
		rc = color.readFrame(&frame);
		if (rc != STATUS_OK)
		{
			printf("Read failed!\n%s\n", OpenNI::getExtendedError());
			continue;
		}

		//check if the frame format is rgb888 frame format
		if (frame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_RGB888)
		{
			printf("Unexpected frame format\n");
			continue;
		}

		int middleIndex = (frame.getHeight() + 1)*frame.getWidth() / 2;

		pColor = (RGB888Pixel*)frame.getData();

		//print the r g b value of the middle pixel of the frame
		printf("[%08llu] 0x%02x%02x%02x\n", (long long)frame.getTimestamp(),
			pColor[middleIndex].r & 0xff,
			pColor[middleIndex].g & 0xff,
			pColor[middleIndex].b & 0xff);
	}

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