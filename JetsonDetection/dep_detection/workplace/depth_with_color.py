import sys
from openni import openni2
import numpy as np
import cv2
sys.path.append('/home/hcm/openni/code/workplace')
import detectnet_mnist  
global dpt

# 鼠标事件
def mousecallback(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDBLCLK:
        print(y, x, dpt[y, x])

def depth_img_getandout():

    frame = depth_stream.read_frame()
    dframe_data = np.array(frame.get_buffer_as_triplet()).reshape([480, 640, 2])
    dpt1 = np.asarray(dframe_data[:, :, 0], dtype='float32')
    dpt2 = np.asarray(dframe_data[:, :, 1], dtype='float32')
    dpt2 *= 255
    global dpt 
    dpt = dpt1 + dpt2
    dpt = dpt[:, ::-1]
    #print(dpt)
    cv2.imshow('depth', dpt)
    return cv2.waitKey(20)






if __name__ == "__main__":
    a=detectnet_mnist.detectionet_class()
    openni2.initialize()
    dev = openni2.Device.open_any()
    depth_stream = dev.create_depth_stream()
    depth_stream.start()
    cv2.namedWindow('depth')
    cv2.setMouseCallback('depth', mousecallback)

    while True:
        a.detection_run()
        
        if int(depth_img_getandout()) == ord('q'):
            break
        if(a.judge_detection()):
            break

    depth_stream.stop()
    dev.close()
