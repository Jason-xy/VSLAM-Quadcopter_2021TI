import os

path = r"C:\Users\Jason\Desktop\temp\xml\0"
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
    new_name=os.path.join(path ,str(i)+".xml")
    os.rename(old_name,new_name)
    i+=1

#os.remove(path + '\\' + 'rename')