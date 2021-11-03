# coding:utf8
# 创建时间：2019年7月29日
# 识别二维码并打印二维码的扫描结果
 
 
import cv2
import pyzbar.pyzbar as pyzbar
import time
#import pyzbar
import V_Display as vd
import V_UCom as com
 
# Send_data
Uart_buf = [0x55,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0xAA]
 
# 创建字典二维码数据集
# 注：二维码格式应为int类型
code_dic ={
    "https://blog.csdn.net/sunny2038/article/details/9170013": "1",
    "https://www.cnblogs.com/ronny/p/4083537.html": "2",
    "https://blog.csdn.net/m0_37857300/article/details/79037988": "3",
    "https://blog.csdn.net/qq_25491201/article/details/51065547": "4",
    "https://www.cnblogs.com/kuangxionghui/p/8351064.html": "5",
    "http://www.slamtec.com/cn/Support#rplidar-a2": "6"
}
 
# 二维码识别函数
# 输入参数：读取图像矩阵
# 返回参数：image标记后的图像矩阵;code_dic[barcodeData]二维码编号int类型;(C_x,C_y：目标二维码中心点坐标)
def decodeDisplay(image):
    barcodes = pyzbar.decode(image)
    code_ID = 0  # 如果返回二维码ID返回为0，则表示未检测到
    for barcode in barcodes:
        # 提取条形码的边界框的位置
        # 画出图像中条形码的边界框
        (x, y, w, h) = barcode.rect
        # 二维码中心坐标
        C_x = int(x+w/2)
        C_y = int(y+h/2)
        cv2.circle(image,(C_x,C_y), 2, (0, 255, 0), 8)  # 做出中心坐标
        cv2.rectangle(image, (x, y), (x + w, y + h), (0, 0, 255), 2)    #做出矩形
 
        # 条形码数据为字节对象，所以如果我们想在输出图像上
        # 画出来，就需要先将它转换成字符串
        barcodeData = barcode.data.decode("utf-8")
        barcodeType = barcode.type
 
        # 绘出图像上条形码的数据和条形码类型
        text = "{} ".format(code_dic[barcodeData])
        cv2.putText(image, text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX,
                    .5, (0, 0, 125), 2)
 
        # 向终端打印条形码数据和条形码类型
        # print("[INFO] Found {} barcode: {}".format(barcodeType, barcodeData))
        print('二维码的ID为：', code_dic[barcodeData])
        code_ID = code_dic[barcodeData]
 
        return image, code_ID,
    return image, code_ID
 
 
if __name__ == '__main__':
    # Setup Usart
    com.init(mode=2)
    # 增加了底层驱动，可以直接通过cv2的0号设备读取摄像头
    cap = cv2.VideoCapture(1)
    cap.set(3, 320)  # 设置摄像头输出宽
    cap.set(4, 240)  # 设置摄像头输出高
    print("start reading video...")
    time.sleep(2.0)
    print("start working")
 
    # 初始化右侧图像显示功能,优化了传输,
    # 增加了imshow('name',frame)函数，与cv2的imshow保持一致
    # 无需指定fps
    vd.init()
 
    while True:
        ret, frame = cap.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        im, ID = decodeDisplay(gray)
 
        vd.imshow(im)
 
 
 
 
 