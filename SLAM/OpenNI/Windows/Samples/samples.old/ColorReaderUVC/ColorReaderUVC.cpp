#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    VideoCapture videoCapture;

    videoCapture.open(0); //打开连接到系统中的第一个UVC摄像头
    if (!videoCapture.isOpened())
    {
        printf("没能打开UVC color摄像头");
        return 1;
    }

    //设置视频编码格式,默认情况下不对数据帧进行压缩
    //当读取1080P等高分辨率视频时，需要设置编码压缩格式，防止出现卡顿
    videoCapture.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));

    //设置分辨率
    videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    while (true)
    {
        Mat frame; //定义一个Mat变量，用于存储每一帧图像
        videoCapture >> frame; //读取当前帧,也可通过videoCapture.read(frame);读取

        Vec3b& bgr = frame.at<Vec3b>(frame.rows / 2, frame.cols / 2);

        //print the r g b value of the middle pixel of the frame
        printf("r = %02d, g = %02d, b = %02d\n",
            bgr[2] & 0xff,
            bgr[1] & 0xff,
            bgr[0] & 0xff);

        waitKey(30); //延时30ms
    }

    return 0;
}
