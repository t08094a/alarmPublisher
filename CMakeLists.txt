cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(root)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/CMakeModules/")

# set the installation prefix to a custom one for testing purpose
# if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#     set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed" CACHE PATH "default install path" FORCE )
#     message(STATUS "Install prefix (overriden): ${CMAKE_INSTALL_PREFIX}")
# endif()

set(CMAKE_BUILD_TYPE Debug)
message( STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}" )

# Note: http://unlogic.co.uk/2012/08/13/cmake-and-gcov/
# lcov and gcovr must be installed
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -Wall -W -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -fprofile-arcs -ftest-coverage") # -Wshadow 
SET(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

add_definitions(-std=c++11)


enable_testing()

add_subdirectory(faxParser)
add_subdirectory(smsGateway)
add_subdirectory(alarmPublisher)
