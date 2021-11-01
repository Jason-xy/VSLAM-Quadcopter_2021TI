    ,,m import sensor, image, time, math, lcd, gc, sys
import KPU as kpu
from machine import UART, Timer
from fpioa_manager import fm

# 黑线
GRAYSCALE_THRESHOLD = [(0, 22)]

# 每个roi为(x, y, w, h)，线检测算法将尝试找到每个roi中最大的blob的质心。
# 然后用不同的权重对质心的x位置求平均值，其中最大的权重分配给靠近图像底部的roi，
# 较小的权重分配给下一个roi，以此类推。
ROIS = [ # [ROI, weight]
        (30, 0, 30, 120, 0.7),
        (80, 0, 30, 120, 0.3),
        (130, 0, 30, 120, 0.1)
       ]
# roi代表三个取样区域，（x,y,w,h,weight）,代表左上顶点（x,y）宽高分别为w和h的矩形，
# weight为当前矩形的权值。采用的QQVGA图像大小为160x120，roi即把图像横分成三个矩形。

# yolo输出
yolo_classid = 0
yolo_confidence = 0

# 控制参数
deflection_distance = 0
deflection_angle = 0
velocity_cmps = 15
velocity_angle = 0


def lcd_show_except(e):
    import uio
    err_str = uio.StringIO()
    sys.print_exception(e, err_str)
    err_str = err_str.getvalue()
    img = image.Image(size=(224,224))
    img.draw_string(0, 10, err_str, scale=1, color=(0xff,0x00,0x00))
    lcd.display(img)

def init():
    # 映射串口引脚
    fm.register(6, fm.fpioa.UART1_RX, force=True)
    fm.register(7, fm.fpioa.UART1_TX, force=True)
    # 初始化串口
    uart = UART(UART.UART1, 115200, read_buf_len=4096)
    # 初始化LCD
    lcd.init(freq=15000000)
    lcd.clear(lcd.WHITE)
    # 初始化sensor
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)# KPU输入必须要RGB565
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(2)
    sensor.set_saturation(2)
    sensor.set_auto_gain(False) # 颜色跟踪必须关闭自动增益
    sensor.set_auto_whitebal(False) # 颜色跟踪必须关闭白平衡
    sensor.run(1)

def senddata(uart, funccode = 0x81, length = 0, yolo_classid = 0, yolo_confidence = 0, deflection_distance = 0, velocity_cmps = 15, velocity_angle = 0, deflection_angle = 0):
    # 校验数据
    sumcheck=0
    add_on_check=0

    if(funccode == 0x80):
        # 飞行控制
        # 数据整理
        deflection_distance /= 10
        deflection_angle_byte = int(deflection_angle).to_bytes(2, 'small')
        deflection_angle_high = deflection_angle_byte[0]
        deflection_angle_low = deflection_angle_byte[1]
        velocity_angle_byte = int(velocity_angle).to_bytes(2, 'small')
        velocity_angle_high = velocity_angle_byte[0]
        velocity_angle_low = velocity_angle_byte[1]
        array=bytes([0xAA, 0x61, int(funccode), int(length), 0, int(deflection_distance), 0, int(velocity_cmps), int(velocity_angle_high), int(velocity_angle_low), int(deflection_angle_high), int(deflection_angle_low)])
    if(funccode == 0x81):
        # 降落
        array=bytes([0xAA, 0x61, int(funccode), int(length)])
    if(funccode == 0x90):
        # 数字识别
        array=bytes([0xAA, 0x61, int(funccode), int(length), int(yolo_classid), int(100 * yolo_confidence)])
    else:
        pass

    # 校验码计算
    for i in array:
        sumcheck += i
        add_on_check += sumcheck
    sumcheck=math.fmod(sumcheck, 256)
    add_on_check=math.fmod(add_on_check, 256)

    # 构造发送数据
    array = array + bytes(int(sumcheck)) + bytes(int(add_on_check))
    uart.write(array)

def main(anchors, labels = None, model_addr="/sd/m.kmodel", uart = UART(UART.UART1, 115200, read_buf_len=4096)):
    try:
        task = None
        task = kpu.load(model_addr)
        kpu.init_yolo2(task, 0.5, 0.3, 5, anchors) # threshold:[0,1], nms_value: [0, 1]
        while(True):
            t_all = time.ticks_ms()
            img = sensor.snapshot()
            img.mean(1)# 模糊消噪
            img.erode(1)# 加粗
            # 巡线输入图像
            img_line = img.crop(x_scale = 0.5, y_scale = 0.5)
            # yolo输入图像
            img_yolo = img.crop((48, 8, 224, 224))
            # 利用颜色识别分别寻找三个矩形区域内的线段
            # 记录三个色块坐标
            px = [0, 0, 0]
            py = [0, 0, 0]

            # 巡线
            for r in ROIS:
                blobs = img_line.find_blobs(GRAYSCALE_THRESHOLD, roi=r[0:4], merge=True)
                # r[0:4] is roi tuple.
                # 找到视野中的线,merge=true,将找到的图像区域合并成一个
                # 目标区域找到直线
                if blobs:
                    # 查找像素最多的blob的索引。
                    largest_blob = 0
                    most_pixels = 0
                    for i in range(len(blobs)):
                    # 目标区域找到的颜色块（线段块）可能不止一个，找到最大的一个，作为本区域内的目标直线
                        if blobs[i].pixels() > most_pixels:
                            most_pixels = blobs[i].pixels()
                            # merged_blobs[i][4]是这个颜色块的像素总数，如果此颜色块像素总数大于
                            # most_pixels，则把本区域作为像素总数最大的颜色块。更新most_pixels和largest_blob
                            largest_blob = i
                    # 在色块周围画一个矩形。
                    img_line.draw_rectangle(blobs[largest_blob].rect())
                    # 将此区域的像素数最大的颜色块画矩形和十字形标记出来
                    img_line.draw_cross(blobs[largest_blob].cx(),
                                blobs[largest_blob].cy())
                    if(r[4] == 0.7):
                        px[0] = blobs[largest_blob].cx()
                        py[0] = blobs[largest_blob].cy()
                    if(r[4] == 0.3):
                        px[1] = blobs[largest_blob].cx()
                        py[1] = blobs[largest_blob].cy()
                    if(r[4] == 0.1):
                        px[2] = blobs[largest_blob].cx()
                        py[2] = blobs[largest_blob].cy()

            # yolo前向传播
            t_yolo = time.ticks_ms()
            objects = kpu.run_yolo2(task, img_yolo)
            t_yolo = time.ticks_ms() - t_yolo
            if objects:
                for obj in objects:
                    yolo_classid = obj.classid()
                    yolo_confidence = obj.value()
                    # pos = obj.rect()
                    # img.draw_rectangle(pos)
                    # img.draw_string(pos[0], pos[1], "%s : %.2f" %(labels[obj.classid()], obj.value()), scale=2, color=(255, 0, 0))

            # 运动控制
            # 调整策略 先让最近的方块位于中心再调整使运动方向与割线平行
            # 平移距离
            deflection_distance = 0 # pixels
            velocity_angle = 0
            if(px[0] != 0 or py[0] != 0):# 检测到第三个色块
                deflection_distance = math.sqrt(math.pow((py[0] - 60), 2) + math.pow(50, 2))
                velocity_angle = math.atan((py[0] - 60) / 50)
                velocity_angle = math.degrees(velocity_angle)

            # 旋转角度
            deflection_angle = 0
            # 计算角度
            if(px[2] == 0 and py[2] == 0):# 第一个色块消失
                # 以第二三个方块为基准
                if(px[1] == 0 and py[1] == 0): # 前两个色块的消失
                    # 巡线完毕或丢失准备降落
                    senddata(uart, funccode = 0x81, length = 0)
                    continue
                else:
                    # 以第二三个方块判断角度
                    deflection_angle = math.atan((py[1] - py[0]) / (px[1] - px[0]))# px[1] != px[0]不会出现非法除0的情况
                    deflection_angle = math.degrees(deflection_angle)
            else:
                deflection_angle = math.atan((py[2] - py[1]) / (px[2] - px[1]))# px[2] != px[1]不会出现非法除0的情况
                deflection_angle = math.degrees(deflection_angle)

            if(deflection_distance < 0):
                velocity_angle = -velocity_angle
                deflection_distance = -deflection_distance
            else:
                velocity_angle = 360 - velocity_angle
            if(deflection_angle > 0):
                deflection_angle = 360 - deflection_angle
            else:
                deflection_angle = -deflection_angle

            # 显示结果
            # 打印原始计算数据
            t_all = time.ticks_ms() - t_all
            lcd.draw_string(0, 0, str(deflection_angle), lcd.WHITE, lcd.BLACK)
            lcd.draw_string(0, 20, str(deflection_distance), lcd.WHITE, lcd.BLACK)
            lcd.draw_string(0, 40, str(velocity_angle), lcd.WHITE, lcd.BLACK)
            lcd.draw_string(0, 60, "classid:%d" %(yolo_classid), lcd.WHITE, lcd.BLACK)
            lcd.draw_string(0, 80, "confidence:%d" %(yolo_confidence), lcd.WHITE, lcd.BLACK)
            lcd.draw_string(0, 100, "t_yolo:%dms" %(t_yolo), lcd.WHITE, lcd.BLACK)
            lcd.draw_string(0, 120, "t_all:%dms" %(t_yolo), lcd.WHITE, lcd.BLACK)
            lcd.display(img_line)

            # 数据报文
            # func_ode: 0x80 移动控制 | 0x81 降落 | 0x90 目标检测
            senddata(uart, func_ode = 0x80, length = 8, deflection_distance = deflection_distance, velocity_cmps = velocity_cmps, velocity_angle = velocity_angle, deflection_angle = deflection_angle)
            senddata(uart, func_ode = 0x90, length = 2, yolo_classid = yolo_classid, yolo_confidence = yolo_confidence)
    except Exception as e:
        raise e
    finally:
        if not task is None:
            kpu.deinit(task)

if __name__ == "__main__":
    try:
        labels = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
        anchors = [3.8125, 3.8125, 5.375, 5.375, 7.1875, 7.1875, 11.25, 11.3125, 9.125, 9.125]
        init()
        main(anchors = anchors, labels=labels, model_addr="/sd/m.kmodel")
        # main(anchors = anchors, labels=labels, model_addr="/sd/m.kmodel")
    except Exception as e:
        sys.print_exception(e)
        lcd_show_except(e)
    finally:
        gc.collect()
