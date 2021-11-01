#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    VideoCapture videoCapture;

    videoCapture.open(0); //�����ӵ�ϵͳ�еĵ�һ��UVC����ͷ
    if (!videoCapture.isOpened())
    {
        printf("û�ܴ�UVC color����ͷ");
        return 1;
    }

    //������Ƶ�����ʽ,Ĭ������²�������֡����ѹ��
    //����ȡ1080P�ȸ߷ֱ�����Ƶʱ����Ҫ���ñ���ѹ����ʽ����ֹ���ֿ���
    videoCapture.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));

    //���÷ֱ���
    videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    while (true)
    {
        Mat frame; //����һ��Mat���������ڴ洢ÿһ֡ͼ��
        videoCapture >> frame; //��ȡ��ǰ֡,Ҳ��ͨ��videoCapture.read(frame);��ȡ

        Vec3b& bgr = frame.at<Vec3b>(frame.rows / 2, frame.cols / 2);

        //print the r g b value of the middle pixel of the frame
        printf("r = %02d, g = %02d, b = %02d\n",
            bgr[2] & 0xff,
            bgr[1] & 0xff,
            bgr[0] & 0xff);

        waitKey(30); //��ʱ30ms
    }

    return 0;
}
