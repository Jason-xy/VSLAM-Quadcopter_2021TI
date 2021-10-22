from lxml.etree import Element, SubElement, tostring
from xml.dom.minidom import parseString
import cv2 as cv
import random
import os
import csv

bg_x = bg_y = 224
num_x = num_y = 28
bg_img_num = 1774
num_img_num = 5500
data_amount = 2000
spin_degrees = (0, 90, 180, 270)
min_resize = 200
max_resize = 500
#image
bg_random_num = 0
num_random_num = 0
spin_random = 0
resize_random = 0
x_random = 0
y_random = 0
transparency_random = 0
#csv
xmin = 0
ymin = 0
xmax = 0
ymax =0

def save_xml(image_name, xmin, ymin, xmax, ymax, class_name, save_dir, width=224, height=224, channel=3):

    node_root = Element('annotation')

    node_folder = SubElement(node_root, 'folder')
    node_folder.text = '%s' % class_name

    node_filename = SubElement(node_root, 'filename')
    node_filename.text = image_name

    node_path = SubElement(node_root, 'path')
    node_path.text = '%s' % os.path.join(save_dir, image_name)

    node_source = SubElement(node_root, 'source')
    node_database = SubElement(node_source, 'database')
    node_database.text = 'Unknown'

    node_size = SubElement(node_root, 'size')
    node_width = SubElement(node_size, 'width')
    node_width.text = '%s' % width

    node_height = SubElement(node_size, 'height')
    node_height.text = '%s' % height

    node_depth = SubElement(node_size, 'depth')
    node_depth.text = '%s' % channel

    node_segmented = SubElement(node_root, 'segmented')
    node_segmented.text = '0'

    node_object = SubElement(node_root, 'object')
    node_name = SubElement(node_object, 'name')
    node_name.text = '%s' % class_name
    node_pose = SubElement(node_object, 'pose')
    node_pose.text = 'Unspecified'
    node_truncated = SubElement(node_object, 'truncated')
    node_truncated.text = '0'
    node_difficult = SubElement(node_object, 'difficult')
    node_difficult.text = '0'
    node_bndbox = SubElement(node_object, 'bndbox')
    node_xmin = SubElement(node_bndbox, 'xmin')
    node_xmin.text = '%s' % xmin
    node_ymin = SubElement(node_bndbox, 'ymin')
    node_ymin.text = '%s' % ymin
    node_xmax = SubElement(node_bndbox, 'xmax')
    node_xmax.text = '%s' % xmax
    node_ymax = SubElement(node_bndbox, 'ymax')
    node_ymax.text = '%s' % ymax

    xml = tostring(node_root, pretty_print=True)  
    dom = parseString(xml)

    save_xml = os.path.join(save_dir, image_name.replace('jpg', 'xml'))
    with open(save_xml, 'wb') as f:
        f.write(xml)
    return

for i in range(10):
    #构建文件路径
    bg_path = r"C:\Users\Jason\Desktop\background"
    num_path = r"C:\Users\Jason\Desktop\MNIST\raw\color_inversion"
    img_dst_path = r"C:\Users\Jason\Desktop\MNIST-VOCdataset\JPEGImages"
    xml_dst_path = r"C:\Users\Jason\Desktop\MNIST-VOCdataset\Annotations"
    num_path = num_path + '\\' + str(i)
    # img_dst_path = img_dst_path + '\\' + str(i)
    # xml_dst_path = xml_dst_path + '\\' + str(i)
    for j in range(data_amount):
        #随机抽取图片与变换方式
        bg_random_num = random.randint(0, bg_img_num)
        num_random_num = random.randint(0, num_img_num)
        spin_random = random.randint(0, 3)
        resize_random = random.randint(min_resize, max_resize)
        x_random = random.randint(0, bg_x - int(num_x * resize_random / 100))
        y_random = random.randint(0, bg_y - int(num_y * resize_random / 100))
        transparency_random = random.randint(55, 80)
        #读入抽取的图片
        bg_img = cv.imread(bg_path + '\\' + str(bg_random_num) + '.jpg')
        bg_img_bak = bg_img.copy()
        # cv.imshow('1', bg_img)
        num_img = cv.imread(num_path + '\\' + str(num_random_num) + '.jpg')
        #构造新数据
        if(num_img is not None):
            num_img = cv.resize(num_img, (int(num_x * (resize_random / 100)), int(num_y * (resize_random / 100))), interpolation=cv.INTER_LINEAR)
            M = cv.getRotationMatrix2D((num_img.shape[0] / 2, num_img.shape[1] / 2), spin_degrees[spin_random], 1)
            num_img = cv.warpAffine(num_img, M, (int(num_x * (resize_random / 100)), int(num_y * (resize_random / 100))))
            # print(num_img.shape[0], num_img.shape[1])
            bg_img[x_random: x_random + num_img.shape[0], y_random: y_random + num_img.shape[1]] = num_img
            dst_img = cv.addWeighted(bg_img, transparency_random / 100, bg_img_bak, 1 - transparency_random / 100, 0)
            xmin = x_random
            ymin = y_random
            xmax = x_random + int(num_x * resize_random / 100)
            ymax = y_random + int(num_y * resize_random / 100)
            save_xml(str(i * data_amount + j) + '.jpg', ymin, xmin, ymax, xmax, str(i), xml_dst_path)
            cv.imwrite(img_dst_path + '\\' + str(i * data_amount + j) + '.jpg', dst_img, [int(cv.IMWRITE_PNG_COMPRESSION), 9])
    #     print('j=' + str(j))
    # print('i=' + str(i))




