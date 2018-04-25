cmake_minimum_required(VERSION 3.2)
project(OpenPSVR VERSION 0.1.0)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeUtils.txt)
include(ExternalProject)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_CXX_STANDARD_17)
set(CMAKE_CXX_STANDARD_REQUIRED_ON)
IF (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-std=c++11")
    IF (NOT CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    ENDIF()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch")
ENDIF()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Configure and build external 3rdParty projects from released packages
if (${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8) # 64-bit
        if(${CMAKE_SYSTEM_NAME} MATCHES "Windows") # Windows
                set(ARCH_LABEL "Win64")
				build_external_project(libusb  ${ROOT_DIR}/deps https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.7z)
				build_external_project(libpsvr ${ROOT_DIR}/deps https://ci.appveyor.com/api/buildjobs/ltvwwd2omqy7vgpd/artifacts/build%2FWin64%2Flibpsvr%2Flibpsvr.zip)
				build_external_project(openvr  ${ROOT_DIR}/deps https://github.com/ValveSoftware/openvr/archive/v1.0.13.zip)
				build_external_project(glm     ${ROOT_DIR}/deps https://github.com/g-truc/glm/releases/download/0.9.8.5/glm-0.9.8.5.7z)
        elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # OSX
				#build_external_project(libusb ${ROOT_DIR}/deps https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.tar.bz2)
				# TODO add rest of dependencies
        else()# Linux
				#build_external_project(libusb ${ROOT_DIR}/deps https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.tar.bz2)
				# TODO add rest of dependencies
        endif()
endif()

add_subdirectory(src)
