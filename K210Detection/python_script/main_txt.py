import os
import random
 
trainval_percent = 0.66
train_percent = 0.5
xmlfilepath = '/home/hcm/Desktop/MNIST-VOCdataset/datasets/Annotations'
txtsavepath = '/home/hcm/Desktop/MNIST-VOCdataset/datasets/ImageSets/Main'
total_xml = os.listdir(xmlfilepath)
num=len(total_xml)
list=range(num)
tv=int(num*trainval_percent)
tr=int(tv*train_percent)
trainval= random.sample(list,tv)
train=random.sample(trainval,tr)
 
ftrainval = open('/home/hcm/Desktop/MNIST-VOCdataset/datasets/ImageSets/Main/trainval.txt', 'w')
ftest = open('/home/hcm/Desktop/MNIST-VOCdataset/datasets/ImageSets/Main/test.txt', 'w')
ftrain = open('/home/hcm/Desktop/MNIST-VOCdataset/datasets/ImageSets/Main/train.txt', 'w')
fval = open('/home/hcm/Desktop/MNIST-VOCdataset/datasets/ImageSets/Main/val.txt', 'w')
 
for i  in list:
    name=total_xml[i][:-4]
    if i in trainval:
        ftrainval.write(name+'\n')
        if i in train:
            ftrain.write(name+'\n')
        else:
            fval.write(name+'\n')
    else:
        ftest.write(name+'\n')
 
ftrainval.close()
ftrain.close()
fval.close()
ftest .close()
