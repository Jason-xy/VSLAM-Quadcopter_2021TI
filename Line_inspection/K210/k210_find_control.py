import sensor,image,time,math
from machine import UART,Timer
from fpioa_manager import fm

GRAYSCALE_THRESHOLD = (60, 255)#赛道的灰度值
TRACK_LINE=(0,1)
MAG_THRESHOLD=4
FORWARD_DISTANCE=5
sensor.reset() # 初始化sensor
sensor.set_pixformat(sensor.GRAYSCALE) # use grayscale.#设置图像色彩格式，有RGB565色彩图和GRAYSCALE灰度图两种
sensor.set_framesize(sensor.QQVGA) # 使用QQVGA的速度。#设置图像像素大小 80x60
sensor.skip_frames(30) # 让新的设置生效。
#sensor.set_auto_gain(False) # 颜色跟踪必须关闭自动增益
#sensor.set_auto_whitebal(False) # 颜色跟踪必须关闭白平衡
sensor.set_brightness(3)
#映射串口引脚
fm.register(6, fm.fpioa.UART1_RX, force=True)
fm.register(7, fm.fpioa.UART1_TX, force=True)
#初始化串口
uart = UART(UART.UART1, 115200, read_buf_len=4096)
while(1):
    img=sensor.snapshot()#提高对比度
    #img.mean(1)#模糊消噪
    img.binary([GRAYSCALE_THRESHOLD])#二值化
    #img.erode(1)#加粗

    line=img.get_regression([TRACK_LINE],robust=True)
    if line and (line.magnitude()>=MAG_THRESHOLD):
        target=line.line()
        img.draw_line(target,color=255)
        #print(target)
        # 计算速度
        velocity_cmps = 10  # 定值10

        # 计算使线平行于航线的角0~180
        if ((target[0] - target[2]) != 0):
            k = (target[1] - target[3]) / (target[0] - target[2])
            angle = math.atan(k) * 57.3
            if (angle < 0):
                angle = 180 + angle
        else:
            angle = 90

        dir_angle_0_360 = 180 - angle;
        # 计算位移向量的距离，在回正的同时前进

        center_point = ((target[0] + target[2]) / 2, (target[1] + target[3]) / 2)
        target_vector_quantity = (40 - center_point[0] + FORWARD_DISTANCE * math.cos(dir_angle_0_360),
                                  30 - center_point[1] - FORWARD_DISTANCE * math.sin(dir_angle_0_360))
        x_2 = target_vector_quantity[0] * target_vector_quantity[0]
        y_2 = target_vector_quantity[1] * target_vector_quantity[1]
        distance_cm = math.sqrt(x_2 + y_2)

        # 计算位移需要的角0~360
        if (target_vector_quantity[0] != 0):
            k = target_vector_quantity[1] / target_vector_quantity[0]
            angle = math.atan(k) * 57.3
            if (angle < 0):
                angle = 180 + angle
        else:
            angle = 90
        #print("############################")
        #angle = angle
        #print(target_vector_quantity)
        #print(angle)
        if (target_vector_quantity[0] < 0 and target_vector_quantity[1] < 0):
            angle = 180 + angle
        elif (target_vector_quantity[0] > 0 and target_vector_quantity[1] < 0):
            angle = 180 + angle

        spin_angle_0_360 = angle
        spin_angle_0_360_byte = int(spin_angle_0_360).to_bytes(2, 'small')
        spin_angle_0_360_high = spin_angle_0_360_byte[0]
        spin_angle_0_360_low = spin_angle_0_360_byte[1]
        #print(spin_angle_0_360)
        #print("############################")

        sumcheck=0
        add_on_check=0
        arry=bytes([0xAA,0x61,0x80,0x08,0,int(distance_cm),0,int(velocity_cmps),0,int(dir_angle_0_360),int(spin_angle_0_360_high),int(spin_angle_0_360_low)])
        for i in arry:
            sumcheck+=i
            add_on_check+=sumcheck
        sumcheck=math.fmod(sumcheck, 256)
        add_on_check=math.fmod(add_on_check, 256)
        arry2=bytes([0xAA,0x61,0x80,0x08,0,int(distance_cm),0,int(velocity_cmps),0,int(dir_angle_0_360),int(spin_angle_0_360_high),int(spin_angle_0_360_low),int(sumcheck),int(add_on_check)])
        uart.write(arry2)
