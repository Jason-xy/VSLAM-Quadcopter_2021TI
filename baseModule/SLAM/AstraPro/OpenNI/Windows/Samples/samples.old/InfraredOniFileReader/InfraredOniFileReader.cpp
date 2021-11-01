#include <stdio.h>
#include <OpenNI.h>

using namespace openni;

int main()
{
	Status rc = OpenNI::initialize();
	if (rc != STATUS_OK)
	{
		printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
		return 1;
	}

	char irPathFile[256] = "ir.oni";
	
	Device device;
	//打开文件设备，参数是已经录制好的ir流的oni文件的路径
	rc = device.open(irPathFile);
	if (rc != STATUS_OK)
	{
		printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());
		return 2;
	}
	
	VideoStream ir;
	
	//创建IR流
	rc = ir.create(device, SENSOR_IR);
	if (rc != STATUS_OK)
	{
		printf("Couldn't create  ir stream\n%s\n", OpenNI::getExtendedError());
		return 3;
	}
	
	//启动ir流
	rc = ir.start();
	if (rc != STATUS_OK)
	{
		printf("Couldn't start the depth stream\n%s\n", OpenNI::getExtendedError());
		return 4;
	}
	
	//获取回放控制对象
	openni::PlaybackControl * irPlaybackCtrl = device.getPlaybackControl();
	
	//获取oni文件设备中的IR流的总帧数
	int totalFrameNum = irPlaybackCtrl->getNumberOfFrames(ir);
	
	for (int k = 0; k < totalFrameNum; k++)
	{
		//寻找ir流中指定索引k的数据帧
		irPlaybackCtrl->seek(ir, k);
		VideoFrameRef frame;
	
		//读取数据帧
		ir.readFrame(&frame);
	
		if (frame.getVideoMode().getPixelFormat() != PIXEL_FORMAT_GRAY16)
		{
			printf("Unexpected frame format\n");
			continue;
		}
	
		Grayscale16Pixel* pIR = (Grayscale16Pixel*)frame.getData();
	
		int middleIndex = (frame.getHeight() + 1)*frame.getWidth() / 2;
	
		printf("[%08llu] %8d\n", (long long)frame.getTimestamp(), pIR[middleIndex]);
	}
	
	ir.stop();
	ir.destroy();
	device.close();
	OpenNI::shutdown();
	
	return 0;
}