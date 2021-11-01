set(TARGET InfraredOniFileRecorder)

project(${TARGET})

include_directories(
    ../ThirdParty/Common
    ../ThirdParty/OpenCV241/Include
    ../ThirdParty/OPENNI2/Include
    ../ThirdParty/UvcSwapper/Include
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
    link_directories(
        ../ThirdParty/OPENNI2/x64-Release
        ../ThirdParty/OpenCV241/x64/Lib
        ../ThirdParty/UvcSwapper/x64-Release
    )
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
    link_directories(
        ../ThirdParty/OPENNI2/x86-Release
        ../ThirdParty/OpenCV241/x86/Lib
        ../ThirdParty/UvcSwapper/x86-Release
    )
endif()

file(GLOB SRC "*.h" "*.cpp")

add_executable(${TARGET} ${SRC})

target_link_libraries(${TARGET} UVC-Swapper OpenNI2 opencv_core2413 opencv_highgui2413 opencv_imgproc2413 opencv_calib3d2413)
