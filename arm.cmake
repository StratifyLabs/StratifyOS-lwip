

#Add sources to the project
set(LWIP_FOLDER ../${LWIP_NAME})

file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/* ${CMAKE_SOURCE_DIR}/${LWIP_NAME}/src/include/* )

set(SOURCES_PREFIX ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)
set(SOS_LIB_SOURCELIST ${SOURCES} ${HEADER_FILES})

#lwip kernel
set(SOS_LIB_TYPE release)
set(SOS_LIB_OPTION kernel)
set(SOS_LIB_INCLUDE_DIRECTORIES include ${LWIP_NAME}/src/include)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-lib-std.cmake)

set(SOS_LIB_TYPE debug)
set(SOS_LIB_INCLUDE_DIRECTORIES include ${LWIP_NAME}/src/include)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-lib-std.cmake)
