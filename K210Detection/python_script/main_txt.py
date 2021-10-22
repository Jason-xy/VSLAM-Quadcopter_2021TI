import os
import random
 
trainval_percent = 0.66
train_percent = 0.7
main_path = r'C:\Users\Jason\Desktop\MNIST-VOCdataset'
xmlfilepath = main_path + r'\Annotations'
txtsavepath = main_path + r'\ImageSets\Main'
total_xml = os.listdir(xmlfilepath)
num=len(total_xml)
list=range(num)
tv=int(num*trainval_percent)
tr=int(tv*train_percent)
trainval= random.sample(list,tv)
train=random.sample(trainval,tr)
 
ftrainval = open(main_path + r'\ImageSets\Main\trainval.txt', 'w')
ftest = open(main_path + r'\ImageSets\Main\test.txt', 'w')
ftrain = open(main_path + r'\ImageSets\Main\train.txt', 'w')
fval = open(main_path + r'\ImageSets\Main\val.txt', 'w')
 
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
