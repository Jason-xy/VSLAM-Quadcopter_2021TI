# coding:utf8
# 创建时间：2019年7月29日
# 识别二维码并打印二维码的扫描结果的网址
 
 
import cv2
from pyzbar.pyzbar import decode
 
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
# 返回参数：image标记后的图像矩阵;code_dic[barcodeData]二维码编号int类型;C_x,C_y：目标二维码中心点坐标
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
 
 
def detect():
    # camera = cv2.VideoCapture('../video/code_video_3.mp4')
    # camera = cv2.imread('../images/code_gather/06.png')
    camera = cv2.VideoCapture(1)
    while True:
        # 读取当前帧
        ret, frame = camera.read()
        # 转为灰度图像
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        im, ID = decodeDisplay(gray)
        cv2.imshow("camera", im)
        cv2.waitKey(5)
 
 
    camera.release()
    cv2.destroyAllWindows()
 
 
if __name__ == '__main__':
    detect()
 