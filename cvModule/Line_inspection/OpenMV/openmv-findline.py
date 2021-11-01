import sensor,image,time,math
from pyb import UART

GRAYSCALE_THRESHOLD = (104, 255)#赛道的灰度值
TRACK_LINE=(0,1)
MAG_THRESHOLD=4
sensor.reset() # 初始化sensor
sensor.set_pixformat(sensor.GRAYSCALE) # use grayscale.#设置图像色彩格式，有RGB565色彩图和GRAYSCALE灰度图两种
sensor.set_framesize(sensor.QQQVGA) # 使用QQVGA的速度。#设置图像像素大小 80x60
sensor.skip_frames(30) # 让新的设置生效。
sensor.set_auto_gain(False) # 颜色跟踪必须关闭自动增益
sensor.set_auto_whitebal(False) # 颜色跟踪必须关闭白平衡
sensor.set_brightness(3)
clock = time.clock() # 跟踪FPS帧率
uart = UART(3, 119200)#串口初始化
while(1):
    clock.tick()
    img=sensor.snapshot()#提高对比度
    img.mean(1)#模糊消噪
    img.binary([GRAYSCALE_THRESHOLD])#二值化
    img.erode(1)#加粗

    line=img.get_regression([TRACK_LINE],robust=True)
    if line and (line.magnitude()>=MAG_THRESHOLD):
        target=line.line()
        img.draw_line(target,color=255)
        print(target)
        center_point=((target[0]+target[2])/2,(target[1]+target[3])/2)
        x_2=(40-center_point[0])*(40-center_point[0])
        y_2=(30-center_point[1])*(30-center_point[1])
        distance_cm=math.sqrt(x_2+y_2)
        velocity_cmps=10 #定值10
        #spin_angle_0_360=
#计算使线平行于航线的角0~180
        if((target[0]-target[2])!= 0):
            k=(target[1]-target[3])/(target[0]-target[2])
            angle=math.atan(k)*57.3
            if(angle<0):
                angle=180+angle
        else:
            angle=90
        dir_angle_0_360=180-angle



#计算位移需要的角0~360
        if(distance_cm>3):
            sin=(center_point[1]-30)/distance_cm
            cos=(center_point[0]-40)/distance_cm

            angle_sin=math.asin(sin)*57.3
            angle_cos=math.acos(cos)*57.3
            if(angle_sin<0 and angle_cos>90):
                angle=360+angle_sin
            elif(angle_sin<0 and angle_cos<90):
                angle=180-angle_sin
            elif(angle_sin>0 and angle_cos>0):
                angle=180-angle_cos
            else:
                angle=angle_cos
        else:
                angle=0
        spin_angle_0_360=angle
        spin_angle_0_360_byte=int(spin_angle_0_360).to_bytes(2,'small')
        spin_angle_0_360_high=spin_angle_0_360_byte[0]
        spin_angle_0_360_low=spin_angle_0_360_byte[1]
        print(spin_angle_0_360_byte)
        print(spin_angle_0_360_high)
        print(spin_angle_0_360_low)




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



    print(clock.fps())