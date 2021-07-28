set(TARGET MultipleStreamRead)

project(${TARGET})

include_directories(
    ../ThirdParty/Common
    ../ThirdParty/openni/Include
    ../ThirdParty/GL
    ../ThirdParty/MWClosestPoint/Include
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
    link_directories(
        ../ThirdParty/openni/x64-Release
        ../ThirdParty/GL
        ../ThirdParty/MWClosestPoint/x64-Debug
    )
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
    link_directories(
        ../ThirdParty/openni/x86-Release
        ../ThirdParty/GL
        ../ThirdParty/MWClosestPoint/x86-Debug
    )
endif()

add_executable(${TARGET} main.cpp)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_link_libraries(${TARGET} MWClosestPoint glut64 OpenNI2)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    target_link_libraries(${TARGET} MWClosestPoint glut32 OpenNI2)
endif()