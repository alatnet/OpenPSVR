cmake_minimum_required(VERSION 3.2)
project(OpenPSVR VERSION 0.1.0)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeUtils.txt)
include(ExternalProject)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(THIRD_PARTY ${ROOT_DIR}/deps)

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

# Configure and build platform specific external 3rdParty projects from their respective released packages
if (${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8) # 64-bit
        if(${CMAKE_SYSTEM_NAME} MATCHES "Windows") # Windows
                download_external_dependency(libusb  ${ROOT_DIR}/deps https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.7z)
				download_external_dependency(libpsvr ${ROOT_DIR}/deps https://ci.appveyor.com/api/buildjobs/ltvwwd2omqy7vgpd/artifacts/build%2FWin64%2Flibpsvr%2Flibpsvr.zip)
				set(LIBPSVR_REQ_LIB ${THIRD_PARTY}/libpsvr/src/libpsvr/libpsvr.lib)
				set(LIBPSVR_INCL_DIR ${THIRD_PARTY}/libpsvr/src/libpsvr)
				set(LIBUSB_REQ_LIB ${THIRD_PARTY}/libusb/src/libusb/MS64/dll/libusb-1.0.lib)
				set(LIBUSB_REQ_SHARED_LIB ${THIRD_PARTY}/libusb/src/libusb/MS64/dll/libusb-1.0.dll)
				set(LIBUSB_INCL_DIR ${THIRD_PARTY}/libusb/src/libusb/include/libusb-1.0)
        elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # OSX
				build_external_project(libusb ${ROOT_DIR}/deps https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.tar.bz2)#
				# TODO add OSX dependencies
        else()# Linux
				build_external_dependecy(libusb 
					${ROOT_DIR}/deps 
					https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.tar.bz2
					${CMAKE_CURRENT_SOURCE_DIR}/deps/libusb/src/libusb/configure
					make -j 2
				)
				download_external_dependency(libpsvr 
					${ROOT_DIR}/deps 
					https://s3-ap-southeast-2.amazonaws.com/libpsvr-snapshot-build/SuperEvenSteven/libpsvr/14/14.1/libpsvr-linux64.tar.gz
				)
				set(LIBPSVR_REQ_LIB ${THIRD_PARTY}/libpsvr/src/libpsvr/build/Linux64/libpsvr/liblibpsvr.so)
				set(LIBPSVR_INCL_DIR ${THIRD_PARTY}/libpsvr/src/libpsvr/include)
				set(LIBUSB_REQ_LIB ${THIRD_PARTY}/libusb/src/libusb-build/libusb/.libs/libusb-1.0.so)
				set(LIBUSB_REQ_SHARED_LIB ${LIBUSB_REQ_LIB})
				set(LIBUSB_INCL_DIR ${THIRD_PARTY}/libusb/src/libusb/libusb)
        endif()
endif()

# OpenVR Cross-platform Dependency
download_external_dependency(openvr
	${ROOT_DIR}/deps 
	https://github.com/ValveSoftware/openvr/archive/v1.0.13.zip)

# GLM Cross-platform Dependency
download_external_dependency(glm     
	${ROOT_DIR}/deps 
	https://github.com/g-truc/glm/releases/download/0.9.8.5/glm-0.9.8.5.7z)
set(GLM_INCL_DIR ${THIRD_PARTY}/glm/src/glm/glm)

add_subdirectory(src)
