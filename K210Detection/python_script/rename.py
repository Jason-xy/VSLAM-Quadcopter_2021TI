import os

j = 0
for j in range(0,10):
    path = "C:\\Users\\Jason\\Desktop\\MNIST-VOCdataset\\datasets\\images\\" + str(j)
    file_list=os.listdir(path)
    os.mkdir(path + '\\' + 'rename')
    i=0
    for fi in file_list:
        old_name=os.path.join(path,fi)
        new_name=os.path.join(path + '\\' + 'rename',str(i)+".jpg")
        os.rename(old_name,new_name)
        i+=1

    file_list=os.listdir(path + '\\' + 'rename')
    i=0
    for fi in file_list:
        old_name=os.path.join(path + '\\' + 'rename',fi)
        new_name=os.path.join(path ,"..\..\..\JPEGImages\\" + str(i + j * 1000)+".jpg")
        os.rename(old_name,new_name)
        i+=1

    # os.remove(path + '\\' + 'rename')

    path = "C:\\Users\\Jason\\Desktop\\MNIST-VOCdataset\\datasets\\xml\\" + str(j)
    file_list=os.listdir(path)
    os.mkdir(path + '\\' + 'rename')
    i=0
    for fi in file_list:
        old_name=os.path.join(path,fi)
        new_name=os.path.join(path + '\\' + 'rename',str(i)+".xml")
        os.rename(old_name,new_name)
        i+=1

    file_list=os.listdir(path + '\\' + 'rename')
    i=0
    for fi in file_list:
        old_name=os.path.join(path + '\\' + 'rename',fi)
        new_name=os.path.join(path ,"..\..\..\Annotations\\" + str(i + j * 1000) +".xml")
        os.rename(old_name,new_name)
        i+=1

    # os.remove(path + '\\' + 'rename')