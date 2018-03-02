

#Add sources to the project
set(LWIP_NAME lwip-2.0.3)
set(LWIP_FOLDER ../${LWIP_NAME})

set(SOURCES_PREFIX ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)
set(SOS_LIB_SOURCELIST ${SOURCES})


# Fatfs kernel for cortex m3 and m4
set(SOS_LIB_TYPE release)
set(SOS_LIB_OPTION kernel)
set(SOS_LIB_ARCH armv7-m)
set(SOS_LIB_INCLUDE_DIRECTORIES include ${LWIP_NAME}/src/include)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-lib.cmake)
set(SOS_LIB_ARCH armv7e-m)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-lib.cmake)
