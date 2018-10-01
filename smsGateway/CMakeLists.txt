cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/CMakeModules/")

# Set options for this project
set(PROJECT_NAME "smsGateway" CXX)
project(${PROJECT_NAME})

message(STATUS "============================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "============================")

set(PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../alarmPublisher/include)
set(PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

add_definitions(-std=c++11)
add_definitions(-DBOOST_LOG_DYN_LINK)

enable_testing()

# Load settings for the external project "Boost".
# The components "system" and "filesystem" from "Boost" version 1.58.0 or
# greater are required.
find_package( Boost 1.58.0 REQUIRED COMPONENTS system log regex date_time filesystem thread )
if (Boost_FOUND)
    # From the offical documentation:
    # Add include directories to the build. [...] If the SYSTEM option is given,
    # the compiler will be told the directories are meant as system include
    # directories on some platforms (signalling this setting might achieve effects
    # such as the compiler skipping warnings [...])."
    include_directories (SYSTEM ${Boost_INCLUDE_DIR})

    message(STATUS "The Boost include directory: ${Boost_INCLUDE_DIR}")
    message(STATUS "The Boost libraries: ${Boost_LIBRARIES})")
    
    # From the offical documentation:
    # "Specify directories in which the linker will look for libraries. [...] Note
    # that this command is rarely necessary. Library locations returned by
    # find_package() and find_library() are absolute paths. Pass these absolute
    # library file paths directly to the target_link_libraries() command. CMake
    # will ensure the linker finds them."
    link_directories (${Boost_LIBRARY_DIRS})
endif ()

aux_source_directory(src SRC_FILES)                                    # find all source files in ./src/

# collect all *.h files to SRC_FILES
file(GLOB HEADER_FILES include/*.h)

message(STATUS "Source files: ${SRC_FILES}")
message(STATUS "Header files: ${HEADER_FILES}")

include_directories (${PROJECT_SOURCE_DECLARATION_DIRECTORY})
include_directories(tests)

add_library(${PROJECT_NAME} ${SRC_FILES})

# 'make install' to the correct location
install(TARGETS ${PROJECT_NAME} DESTINATION alarmPublisher)
#install(FILES ${HEADER_FILES} DESTINATION include)

add_subdirectory(tests)

target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES} curl)
