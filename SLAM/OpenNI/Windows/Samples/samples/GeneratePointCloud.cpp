#include <iostream>
#include <sstream>
#include "OpenNI.h"
#include "OniSampleUtilities.h"

using namespace openni;

#define MIN_DISTANCE 20  //��λ����
#define MAX_DISTANCE 4000 //��λ����
#define RESOULTION_X 640.0  //�궨ʱ�ķֱ���
#define RESOULTION_Y 480.0  //�궨ʱ�ķֱ���

#define MAX_FRAME_COUNT 50

typedef struct xnIntrinsic_Params
{
	xnIntrinsic_Params() :
		c_x(320.0), c_y(240.0), f_x(480.0), f_y(480.0)
	{}

	xnIntrinsic_Params(float c_x_, float c_y_, float f_x_, float f_y_) :
		c_x(c_x_), c_y(c_y_), f_x(f_x_),f_y(f_y_)
	{}
	
	float c_x; //u���ϵĹ�һ������
	float c_y; //v���ϵĹ�һ������
	float f_x; //����x����
	float f_y; //����y����
}xIntrinsic_Params;

xIntrinsic_Params g_IntrinsicParam; //�洢����ڲε�ȫ�ֱ���

void getCameraParams(openni::Device& Device, xIntrinsic_Params& IrParam)
{
	OBCameraParams cameraParam;
	int dataSize = sizeof(cameraParam);
	memset(&cameraParam, 0, sizeof(cameraParam));
	openni::Status rc = Device.getProperty(openni::OBEXTENSION_ID_CAM_PARAMS, (uint8_t *)&cameraParam, &dataSize);
	if (rc != openni::STATUS_OK)
	{
		std::cout << "Error:" << openni::OpenNI::getExtendedError() << std::endl;
		return;
	}
	IrParam.f_x = cameraParam.l_intr_p[0]; //u���ϵĹ�һ������
	IrParam.f_y = cameraParam.l_intr_p[1]; //v���ϵĹ�һ������
	IrParam.c_x = cameraParam.l_intr_p[2]; //����x����
	IrParam.c_y = cameraParam.l_intr_p[3]; //����y����

	std::cout << "IrParam.f_x = " << IrParam.f_x << std::endl;
	std::cout << "IrParam.f_y = " << IrParam.f_y << std::endl;
	std::cout << "IrParam.c_x = " << IrParam.c_x << std::endl;
	std::cout << "IrParam.c_y = " << IrParam.c_y << std::endl;

}

void convertDepthToPointCloud(const uint16_t *pDepth, int width, int height,const char *ply_filename)
{
	if (NULL == pDepth)
	{
		printf("depth frame is NULL!");
		return;
	}
	//���������ת��Ϊ��������,�����������ݱ��浽�ļ���
	FILE *fp;
	
	//int res = fopen_s(&fp, ply_filename, "w");
	fp = fopen(ply_filename, "w");
	
	int valid_count = 0;
	uint16_t max_depth = MAX_DISTANCE;
	uint16_t min_depth = MIN_DISTANCE;
	
	//ͳ����Ч����������
	int img_size = width * height;
	for (int v = 0; v < height; ++v)
	{
		for (int u = 0; u < width; ++u)
		{
			uint16_t depth = pDepth[v * width + u];
			if (depth <= 0 || depth < min_depth || depth > max_depth)
				continue;
	
			valid_count += 1;
		}
	}
	
	//ply�����ļ���ͷ
	fprintf(fp, "ply\n");
	fprintf(fp, "format ascii 1.0\n");
	fprintf(fp, "element vertex %d\n", valid_count);
	fprintf(fp, "property float x\n");
	fprintf(fp, "property float y\n");
	fprintf(fp, "property float z\n");
	fprintf(fp, "property uchar red\n");
	fprintf(fp, "property uchar green\n");
	fprintf(fp, "property uchar blue\n");
	fprintf(fp, "end_header\n");
	
	float world_x, world_y, world_z;
	for (int v = 0; v < height; ++v)
	{
		for (int u = 0; u < width; ++u)
		{
			uint16_t depth = pDepth[v * width + u];
			if (depth <= 0 || depth < min_depth || depth > max_depth)
				continue;
	
			//�ֱ������ţ��������궨ʱ�ķֱ��ʷ�RESOULTION_X��RESOULTION_Y
			float fdx = g_IntrinsicParam.f_x * ((float)(width) / RESOULTION_X);
			float fdy = g_IntrinsicParam.f_y * ((float)(height) / RESOULTION_Y);
			float u0 = g_IntrinsicParam.c_x * ((float)(width)/ RESOULTION_X);
			float v0 = g_IntrinsicParam.c_y * ((float)(height) / RESOULTION_Y);
	
			float tx = (u - u0) / fdx;
			float ty = (v - v0) / fdy;
			
			world_x = depth * tx;
			world_y = depth * ty;
			world_z = depth;
			fprintf(fp, "%f %f %f 255 255 255\n", world_x, world_y, world_z);
		}
	}
	
	fclose(fp);
}

int g_imageCount = 0;

void analyzeFrame(const VideoFrameRef& frame)
{
	DepthPixel* pDepth;

	//��������ļ���
	char plyFileName[256] = "";
	g_imageCount++;
	if (MAX_FRAME_COUNT < g_imageCount)
	{
		return;
	}
	std::stringstream filename;
	filename << "pointcloud_";
	filename << g_imageCount;
	filename << ".ply";
	filename >> plyFileName;
	
	int middleIndex = (frame.getHeight() + 1)*frame.getWidth() / 2;
	
	switch (frame.getVideoMode().getPixelFormat())
	{
	case PIXEL_FORMAT_DEPTH_1_MM:
		pDepth = (DepthPixel*)frame.getData();
		printf("[%08llu] %8d\n", (long long)frame.getTimestamp(),
			pDepth[middleIndex]);
	
		//���������ת��Ϊ���Ʋ������ply�ļ���ÿ֡������ݶ�Ӧһ��ply�ļ�
		convertDepthToPointCloud(pDepth, frame.getWidth(), frame.getHeight(), plyFileName);
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
	
	//get intrinsic parameter from device
	getCameraParams(device, g_IntrinsicParam);
	
	// Wait while we're getting frames through the printer
	while (MAX_FRAME_COUNT > g_imageCount)
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