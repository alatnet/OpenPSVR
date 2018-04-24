cmake_minimum_required(VERSION 3.0)

# initialise variables
set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set(PROJECT_SRC_DIR ${ROOT_DIR}/src)
set(TARGET_BUILD_DIR ${ROOT_DIR}/build)

# 3rd Party library and header paths
set(THIRD_PARTY ${ROOT_DIR}/3rdParty)
set(OPENVR_INCL_DIRS ${THIRD_PARTY}/openvr/headers)
set(GLM_INCL_DIRS ${THIRD_PARTY}/glm/glm)
set(LIBPSVR_INCL_DIRS ${THIRD_PARTY}/libpsvr/include)
set(LIBUSB_INCL_DIRS ${THIRD_PARTY}/libusb/libusb)
set(OPENVR_REQ_LIBS ${THIRD_PARTY}/openvr/lib/linux64)

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

# Set OpenPSVR's required include directories
target_include_directories(OpenPSVR PUBLIC ${OPENVR_INCL_DIRS} 
                                           ${GLM_INCL_DIRS}
                                           ${LIBUSB_INCL_DIRS}
                                           ${LIBPSVR_INCL_DIRS}
)

# Set OpenPSVR's required libraries
target_link_libraries(OpenPSVR ${OPENVR_REQ_LIBS})

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
        RUNTIME DESTINATION ${TARGET_BUILD_DIR}/${ARCH_LABEL}/openpsvr
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

