import cv2
import os

path = r"C:\Users\Jason\Desktop\detection_dataset\mnist\MNIST\raw\train"
file_list = os.listdir(path)

for fi in file_list:
    name = os.path.join(path,fi)
    img = cv2.imread(name)
    img = cv2.bitwise_not(img)
    cv2.imwrite(path + '\color_inversion\\' + fi, img)