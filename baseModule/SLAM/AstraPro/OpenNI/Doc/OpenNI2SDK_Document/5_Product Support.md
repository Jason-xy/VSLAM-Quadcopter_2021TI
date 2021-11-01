# Product Support

# FAQ

## Questions about possibility

###  Q: Whether the OpenNI2 SDK supports color camera of UVC protocol

Currently not supported, for the color camera in the sensor is a UVC protocol device, it is recommended to use the VideoCapture class provided in OpenCV to open the color camera and read the color stream.

------



## Questions about using

###  Q: "load_library failed: 87" on windows 7

Maybe you should install "Windows6.1-KB2533623" patch on your OS.

###  Q: Unity SDK demo and Windows vs2013/2015 x86 demo can't work, but Windows vs2013/2015 x64 demo works.

Install vc++ x86 runtime libraries for vs2013/vs2015.

------


# Legal stuff

## Copyright

Copyright (c) 2015-2020 Orbbec

## Acknowledgements  

This software is based on following open source projects.

- libjpeg http://www.ijg.org/
- openni2 https://github.com/OpenNI/OpenNI2

