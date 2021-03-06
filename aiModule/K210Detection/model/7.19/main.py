# object detector boot.py
# generated by maixhub.com

import sensor, image, lcd, time, math
import KPU as kpu
import gc, sys
from machine import UART, Timer
from fpioa_manager import fm

def lcd_show_except(e):
    import uio
    err_str = uio.StringIO()
    sys.print_exception(e, err_str)
    err_str = err_str.getvalue()
    img = image.Image(size=(224,224))
    img.draw_string(0, 10, err_str, scale=1, color=(0xff,0x00,0x00))
    lcd.display(img)

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
    array = array + bytes([int(sumcheck),int(add_on_check)])
    # array = array.append(byte(int(sumcheck)))
    uart.write(array)
    # print(array)

def main(anchors, labels = None, model_addr="/sd/m.kmodel", sensor_window=(224, 224), lcd_rotation=0, sensor_hmirror=False, sensor_vflip=False, uart = UART(UART.UART1, 115200, read_buf_len=4096)):
    # 映射串口引脚
    fm.register(6, fm.fpioa.UART1_RX, force=True)
    fm.register(7, fm.fpioa.UART1_TX, force=True)
    # 初始化串口
    uart = UART(UART.UART1, 115200, read_buf_len=4096)
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_windowing(sensor_window)
    sensor.set_hmirror(sensor_hmirror)
    sensor.set_vflip(sensor_vflip)
    sensor.run(1)

    lcd.init(type=1)
    lcd.rotation(lcd_rotation)
    lcd.clear(lcd.WHITE)

    if not labels:
        with open('labels.txt','r') as f:
            exec(f.read())
    if not labels:
        print("no labels.txt")
        img = image.Image(size=(320, 240))
        img.draw_string(90, 110, "no labels.txt", color=(255, 0, 0), scale=2)
        lcd.display(img)
        return 1

    try:
        task = None
        task = kpu.load(model_addr)
        kpu.init_yolo2(task, 0.5, 0.3, 5, anchors) # threshold:[0,1], nms_value: [0, 1]
        while(True):
            yolo_classid = 0
            yolo_confidence = 0
            img = sensor.snapshot()
            t = time.ticks_ms()
            objects = kpu.run_yolo2(task, img)
            t = time.ticks_ms() - t
            if objects:
                for obj in objects:
                    yolo_classid = obj.classid()
                    yolo_confidence = obj.value()
                    pos = obj.rect()
                    img.draw_rectangle(pos)
                    img.draw_string(pos[0], pos[1], "%s : %.2f" %(labels[obj.classid()], obj.value()), scale=2, color=(255, 0, 0))
            img.draw_string(0, 200, "t:%dms" %(t), scale=2, color=(255, 0, 0))
            lcd.display(img)
            senddata(uart, funccode = 0x90, length = 2, yolo_classid = yolo_classid, yolo_confidence = yolo_confidence)
    except Exception as e:
        raise e
    finally:
        if not task is None:
            kpu.deinit(task)


if __name__ == "__main__":
    try:
        labels = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
        anchors = [3.8125, 3.8125, 5.375, 5.375, 7.1875, 7.1875, 11.25, 11.3125, 9.125, 9.125]
        main(anchors = anchors, labels=labels, model_addr="/sd/m.kmodel", lcd_rotation=2, sensor_window=(224, 224))
        # main(anchors = anchors, labels=labels, model_addr="/sd/m.kmodel")
    except Exception as e:
        sys.print_exception(e)
        lcd_show_except(e)
    finally:
        gc.collect()
