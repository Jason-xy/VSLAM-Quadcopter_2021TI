#include <iostream>
#include <iomanip>
#include <OpenNI.h>

#define SAMPLE_READ_WAIT_TIMEOUT 2000 //2000ms
#define MAX_RECORD_FRAME_NUM 100

using namespace openni;

int main()
{
	Status rc = OpenNI::initialize();
	if (rc != STATUS_OK)
	{
		std::cout << "Initialize failed\n" << OpenNI::getExtendedError() << std::endl;
		return rc;
	}

	Device device;
	rc = device.open(ANY_DEVICE);
	if (rc != STATUS_OK)
	{
		std::cout << "Couldn't open device\n" << OpenNI::getExtendedError() << std::endl;
		return rc;
	}
	
	VideoStream ir;
	
	if (device.getSensorInfo(SENSOR_IR) != NULL)
	{
		rc = ir.create(device, SENSOR_IR);
		if (rc != STATUS_OK)
		{
			std::cout << "Couldn't create  ir stream\n" << OpenNI::getExtendedError() << std::endl;
			return rc;
		}
	}
	
	rc = ir.start();
	if (rc != STATUS_OK)
	{
		std::cout << "Couldn't start the depth stream\n" << OpenNI::getExtendedError() << std::endl;
		return rc;
	}
	
	//定义一个录制器对象
	Recorder recordIr;
	
	char irPathFile[256] = "ir.oni";//irPathFile为oni文件保存的路径
	
	//创建录制器，需要一个oni文件路径作为参数
	rc = recordIr.create(irPathFile);
	if (STATUS_OK != rc)
	{
		std::cout << "create ir video recorder failed." << std::endl;
		return rc;
	}
	
	//将IR流添加到录制器上
	rc = recordIr.attach(ir);
	if (STATUS_OK != rc)
	{
		std::cout << "attach ir video recorder failed." << std::endl;
		return rc;
	}
	
	//启动录制器
	rc = recordIr.start();
	if (STATUS_OK != rc)
	{
		std::cout << "start ir video recorder failed." << std::endl;
		return rc;
	}
	
	VideoFrameRef frame;
	
	int count = 0;
	while (MAX_RECORD_FRAME_NUM > count)
	{
		int changedStreamDummy;
		VideoStream* pStream = &ir;
		rc = OpenNI::waitForAnyStream(&pStream, 1, &changedStreamDummy, SAMPLE_READ_WAIT_TIMEOUT);
		if (rc != STATUS_OK)
		{
			std::cout << "Wait failed! (timeout is " << SAMPLE_READ_WAIT_TIMEOUT << "ms)\n" << OpenNI::getExtendedError();
			continue;
		}
	
		rc = ir.readFrame(&frame);
		if (rc != STATUS_OK)
		{
			std::cout << "Read failed!\n" << OpenNI::getExtendedError() << std::endl;
			continue;
		}
	
		if (frame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_GRAY16)
		{
			std::cout << "Unexpected frame format\n";
			continue;
		}
	
		Grayscale16Pixel* pIR = (Grayscale16Pixel*)frame.getData();
	
		int middleIndex = (frame.getHeight() + 1)*frame.getWidth() / 2;
	
		std::cout << "[" << std::setw(8) << std::setiosflags(std::ios::right) << std::setfill('0') << (long long)frame.getTimestamp() << "]" << " ";
		std::cout << std::setw(8) << std::setfill(' ') << pIR[middleIndex] << std::endl;
	
		count++;
	}
	
	//关闭及销毁录制器
	recordIr.stop();
	recordIr.destroy();
	
	ir.stop();
	ir.destroy();
	device.close();
	OpenNI::shutdown();
	
	return 0;
}