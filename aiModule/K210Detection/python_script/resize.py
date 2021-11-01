import os
from PIL import Image
import matplotlib.pyplot as plt

path = r"C:\Users\Jason\Desktop\detection_dataset\background\Whiteboard"
file_list=os.listdir(path)

for fi in file_list:
    name=os.path.join(path,fi)
    img=Image.open(name)
    img = img.resize((224, 224))
    # img1=img2=img3=img.convert('L')
    # img=Image.merge('RGB', (img1,img2,img3))
    img.save(name)

    