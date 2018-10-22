cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Set options for this project
set (PROJECT_NAME "alarmPublisher_tests" CXX)
project(${PROJECT_NAME})

message(STATUS "============================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "============================")

set (PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR})
set (PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR})

set(CMAKE_BUILD_TYPE Debug)
message( STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}" )

add_definitions(-std=c++11)
add_definitions(-DBOOST_LOG_DYN_LINK)

enable_testing()

# Load settings for the external project "Boost".
# The components "system" and "filesystem" from "Boost" version 1.58.0 or
# greater are required.
find_package( Boost 1.58.0 REQUIRED COMPONENTS log system filesystem regex date_time unit_test_framework )
if (Boost_FOUND)
    # From the offical documentation:
    # Add include directories to the build. [...] If the SYSTEM option is given,
    # the compiler will be told the directories are meant as system include
    # directories on some platforms (signalling this setting might achieve effects
    # such as the compiler skipping warnings [...])."
    include_directories (SYSTEM ${Boost_INCLUDE_DIR})
    
    message(STATUS "The Boost include directory: ${Boost_INCLUDE_DIR}")
    
    # From the offical documentation:
    # "Specify directories in which the linker will look for libraries. [...] Note
    # that this command is rarely necessary. Library locations returned by
    # find_package() and find_library() are absolute paths. Pass these absolute
    # library file paths directly to the target_link_libraries() command. CMake
    # will ensure the linker finds them."
    link_directories (${Boost_LIBRARY_DIRS})

    message(STATUS "Boost libs: ${Boost_LIBRARIES}")
endif ()

# set(SOURCE ${SOURCE}   ${CMAKE_CURRENT_SOURCE_DIR}../src/*.cpp    PARENT_SCOPE)
# set(HEADERS ${HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}../include/*.h PARENT_SCOPE)

#include_directories(${alarmPublisher_tests_SOURCE_DIR} ${alarmPublisher_tests_SOURCE_DIR}/../src ${alarmPublisher_tests_SOURCE_DIR}/../include/)

# find_library(ALARMPUBLISHER_EXE alarmPublisher PATHS ${CMAKE_BINARY_DIR})
# message(STATUS "========ALARMPUBLISHER_EXE: ${ALARMPUBLISHER_EXE}")

add_executable(${PROJECT_NAME}_ConfigReader TestRunner.cpp ConfigReaderTest.cpp ../src/ConfigReader.cpp) 
target_link_libraries(${PROJECT_NAME}_ConfigReader ${Boost_LIBRARIES})
add_test(NAME ConfigReader COMMAND ${PROJECT_NAME}_ConfigReader)

# add_executable(${PROJECT_NAME}_AlarmMonitorSender TestRunner.cpp AlarmMonitorSenderTest.cpp ../src/AlarmMonitorSender.cpp)
# target_link_libraries(${PROJECT_NAME}_AlarmMonitorSender PRIVATE faxparser ${Boost_LIBRARIES} ${RESTCLIENTCPP})
# add_test(NAME AlarmMonitorSender COMMAND ${PROJECT_NAME}_AlarmMonitorSender)
