cmake_minimum_required(VERSION 3.0)

# initialise variables
set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(PROJECT_SRC_DIR ${ROOT_DIR}/src)
set(TARGET_BUILD_DIR ${ROOT_DIR}/build)
set(THIRD_PARTY ${ROOT_DIR}/deps)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Set OpenPSVR's project source files
add_library(OpenPSVR SHARED ${PROJECT_SRC_DIR}/BMI055Integrator.cpp 
                            ${PROJECT_SRC_DIR}/BMI055Integrator.h
                            ${PROJECT_SRC_DIR}/BMI055MadgwickAHRS.cpp
                            ${PROJECT_SRC_DIR}/BMI055MadgwickAHRS.h
                            ${PROJECT_SRC_DIR}/driverlog.cpp
                            ${PROJECT_SRC_DIR}/driverlog.h
                            ${PROJECT_SRC_DIR}/MadgwickAHRS.cpp
                            ${PROJECT_SRC_DIR}/MadgwickAHRS.h
                            ${PROJECT_SRC_DIR}/MahonyAHRS.cpp
                            ${PROJECT_SRC_DIR}/MahonyAHRS.h
                            ${PROJECT_SRC_DIR}/OpenPSVR.cpp
                            ${PROJECT_SRC_DIR}/OpenPSVR.h
                            ${PROJECT_SRC_DIR}/OpenPSVRDeviceDriver.cpp
                            ${PROJECT_SRC_DIR}/OpenPSVRDeviceDriver.h
                            ${PROJECT_SRC_DIR}/ServerDriver_OpenPSVR.cpp
                            ${PROJECT_SRC_DIR}/ServerDriver_OpenPSVR.h
                            ${PROJECT_SRC_DIR}/WatchdogDriver_OpenPSVR.cpp
                            ${PROJECT_SRC_DIR}/WatchdogDriver_OpenPSVR.h
)

###############################################################################################
# Looks up the required include, library and binary files for each respective external project#
###############################################################################################

#OpenVR references
FIND_PACKAGE(OpenVR REQUIRED)

# Set OpenPSVR's required 3rdParty include directories
set(GLM_INCL_DIR ${THIRD_PARTY}/glm/src/glm/glm)
set(LIBPSVR_INCL_DIR ${THIRD_PARTY}/libpsvr/src/libpsvr)
set(LIBUSB_INCL_DIR ${THIRD_PARTY}/libusb/src/libusb/include/libusb-1.0)
target_include_directories(OpenPSVR PUBLIC ${OPENVR_INCLUDE_DIR} 
                                           ${GLM_INCL_DIR}
                                           ${LIBPSVR_INCL_DIR}
										   ${LIBUSB_INCL_DIR}
)

#find_path(LIBUSB_INCLUDE_DIR
#  NAMES libusb.h
#  PATH_SUFFIXES "include" "libusb" "libusb-1.0")
#find_library(LIBUSB_LIBRARY
#  NAMES usb
#  PATH_SUFFIXES "lib" "lib32" "lib64")
    
# Set OpenPSVR's required 3rdParty libraries
#set(OPENVR_REQ_LIB ${THIRD_PARTY}/openvr/lib/win64/openvr_api.lib)
set(LIBPSVR_REQ_LIB ${THIRD_PARTY}/libpsvr/src/libpsvr/libpsvr.lib)
set(LIBUSB_REQ_LIB ${THIRD_PARTY}/libusb/src/libusb/MS64/dll/libusb-1.0.lib)
target_link_libraries(OpenPSVR ${OPENVR_LIBRARIES}
                               ${LIBPSVR_REQ_LIB}
							   ${LIBUSB_REQ_LIB}
)

# Shared architecture label used for install folder locations
if (${CMAKE_C_SIZEOF_DATA_PTR} EQUAL 8)
        if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
                set(ARCH_LABEL "Win64")
        elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
                set(ARCH_LABEL "OSX64")
        else()
                set(ARCH_LABEL "Linux64")
        endif()
else()
        if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
                set(ARCH_LABEL "Win32")
        elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
                set(ARCH_LABEL "OSX32")
        else()
                set(ARCH_LABEL "Linux32")
        endif()
endif()

# Define OpenPSVR install target
install(TARGETS OpenPSVR
        RUNTIME DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/openpsvr/bin/${ARCH_LABEL}
        LIBRARY DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/lib
        ARCHIVE DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/lib
)

# Copy driver vr manifest
install(
        FILES ${ROOT_DIR}/driver/driver.vrdrivermanifest
        DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/openpsvr
)

# Copy driver icon resources
install(
        DIRECTORY ${ROOT_DIR}/driver/resources/icons
        DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/openpsvr/resources
        FILES_MATCHING PATTERN "*.png" 
)

#  Copy the driver default settings
install(
        DIRECTORY ${ROOT_DIR}/driver/resources/settings
        DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/openpsvr/resources
        FILES_MATCHING PATTERN "*.vrsettings" 
)

#  Copy the libusb library
install(
        FILES ${LIBUSB_REQ_LIB}
        DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/openpsvr/bin/${ARCH_LABEL}
)

