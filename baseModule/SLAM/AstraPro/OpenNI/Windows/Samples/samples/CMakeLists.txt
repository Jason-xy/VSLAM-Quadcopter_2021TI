# ubuntu 12.04 LTS cmake version 2.8.7
# ubuntu 14.04 LTS cmake version 2.8.12.2
# ubuntu 16.04 LTS cmake version 3.5.1
cmake_minimum_required(VERSION 2.8.3)
#规定cmake程序的最低版本。这行命令是可选的，我们可以不写这句话，但在有些情况下，
#如果CMakeLists.txt文件中使用了一些高版本cmake特有的一些命令的时候，就需要加上这样一行，
#提醒用户升级到该版本之后再执行cmake。

project(OpenNI2Examples)

# Save the command line compile commands in the build output
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
# View the makefile commands during build
#set(CMAKE_VERBOSE_MAKEFILE on)

#build type (Debug or Release)
SET(CMAKE_BUILD_TYPE Release)

# c++ examples
# include header
if(WIN32)

include_directories(../thirdParty/OpenNI2/Include ./Common ../thirdParty/OpenCV/windows/opencv2413/include)

elseif(UNIX)

include_directories(../thirdParty/OpenNI2/Include ./Common ../thirdParty/OpenCV/linux/Include)

else()
#other os
  MESSAGE(STATUS "other os")  
endif()

#platform x64 or x86
#if(WIN32)
#    if(CMAKE_CL_64)  
#       SET(PLATFORM x64)      
#    else(CMAKE_CL_64)  
#       SET(PLATFORM x86)  
#    endif(CMAKE_CL_64)  
#else(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
       SET(PLATFORM x64)
    else(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(PLATFORM x86)
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8) 
#endif(Win32)

#include libs dir
if(WIN32)
#windows
	SET(LIB_DIRECTORY ${PROJECT_SOURCE_DIR}/../thirdParty/OpenNI2/windows/libs/release/${PLATFORM})
	link_directories(${LIB_DIRECTORY}  ${LIB_DIRECTORY}/OpenNI2/Drivers/)
	
	SET(OPENCV_LIB_DIRECTORY ${PROJECT_SOURCE_DIR}/../thirdParty/OpenCV/windows/opencv2413/${PLATFORM}/vc14/lib)
	link_directories(${OPENCV_LIB_DIRECTORY} )

elseif(UNIX)
#linux
#	SET(LIB_DIRECTORY ${PROJECT_SOURCE_DIR}/../linux/libs/release/intel/x64)
	SET(LIB_DIRECTORY ${PROJECT_SOURCE_DIR}/../thirdParty/OpenNI2/linux/libs/release/intel/${PLATFORM})
    link_directories(${LIB_DIRECTORY}  ${LIB_DIRECTORY}/OpenNI2/Drivers/)
	
	#SET(OPENCV_LIB_DIRECTORY ${PROJECT_SOURCE_DIR}/../thirdParty/OpenCV/linux/lib)
	#link_directories(${OPENCV_LIB_DIRECTORY} )
find_package(OpenCV 3.2 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

else()
#other os
  MESSAGE(STATUS "other os")  
endif()

#
#find_library(OpenNI2 orbbec OniFile ../libs/release/intel/x64/*)
#link_libraries(libOpenNI2.so liborbbec.so libOniFile.so)

#set dependency lib
if(WIN32)
	SET(ORBBEC_LIB OpenNI2)
	SET(OPENCV2413_LIB opencv_core2413 opencv_highgui2413 opencv_imgproc2413 opencv_calib3d2413 )

else(WIN32)
	SET(ORBBEC_LIB OpenNI2 orbbec OniFile)
	#SET(OPENCV2413_LIB opencv_core opencv_highgui opencv_imgproc opencv_calib3d opencv_videoio)
endif(WIN32)

#ColorReaderPoll example
add_executable(ColorReaderPoll ColorReaderPoll.cpp )
target_link_libraries(ColorReaderPoll ${ORBBEC_LIB})

#ColorReaderEvent example
add_executable(ColorReaderEvent ColorReaderEvent.cpp)
target_link_libraries(ColorReaderEvent ${ORBBEC_LIB})

#DepthReaderPoll example
add_executable(DepthReaderPoll DepthReaderPoll.cpp)
target_link_libraries(DepthReaderPoll ${ORBBEC_LIB})

#DepthReaderEvent example
add_executable(DepthReaderEvent DepthReaderEvent.cpp)
target_link_libraries(DepthReaderEvent ${ORBBEC_LIB})

#InfraredReaderPoll example
add_executable(InfraredReaderPoll InfraredReaderPoll.cpp)
target_link_libraries(InfraredReaderPoll ${ORBBEC_LIB})

#InfraredReaderEvent example
add_executable(InfraredReaderEvent InfraredReaderEvent.cpp)
target_link_libraries(InfraredReaderEvent ${ORBBEC_LIB})

#GeneratePointCloud example
add_executable(GeneratePointCloud GeneratePointCloud.cpp)
target_link_libraries(GeneratePointCloud ${ORBBEC_LIB})

if(WIN32)
#ColorReaderUVC example
add_executable(ColorReaderUVC ColorReaderUVC.cpp)
target_link_libraries(ColorReaderUVC ${ORBBEC_LIB} ${OPENCV2413_LIB})
else(WIN32)
add_executable(ColorReaderUVC ColorReaderUVC.cpp)
target_link_libraries(ColorReaderUVC ${ORBBEC_LIB} ${OpenCV_LIBS})
endif(WIN32)

#InfraredOniFileRecorder.cpp example
add_executable(InfraredOniFileRecorder InfraredOniFileRecorder.cpp)
target_link_libraries(InfraredOniFileRecorder ${ORBBEC_LIB})

#InfraredOniFileReader example
add_executable(InfraredOniFileReader InfraredOniFileReader.cpp)
target_link_libraries(InfraredOniFileReader ${ORBBEC_LIB})



