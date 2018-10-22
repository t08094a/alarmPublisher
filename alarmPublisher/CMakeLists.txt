cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/CMakeModules/")

# Set options for this project
set(PROJECT_NAME "alarmPublisher" CXX)
project(${PROJECT_NAME})

add_definitions(-std=c++11)

message(STATUS "============================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "============================")

SET(PROJECT_VER_MAJOR 1)
SET(PROJECT_VER_MINOR 1)
SET(PROJECT_VER_PATCH 0)
SET(PROJECT_VER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
SET(PROJECT_APIVER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}")

set(PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

include_directories (${PROJECT_SOURCE_DECLARATION_DIRECTORY})
include_directories(tests)

add_subdirectory(tests)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
#target_include_directories(${PROJECT_NAME} PUBLIC include)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

# Load settings for the external project "Boost".
# The components "system" and "filesystem" from "Boost" version 1.58.0 or
# greater are required.
find_package( Boost 1.58.0 REQUIRED COMPONENTS system date_time log log_setup )
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


# find all source files in ./src/
aux_source_directory(src SRC_FILES)
aux_source_directory(include HEADER_FILES)

message(STATUS "Source files: ${SRC_FILES}")

#add_subdirectory(tests)

# Add dependencies to include and lib for faxparser and external projects
include_directories(${faxparser_SOURCE_DIR} ${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

include_directories(tests)

# get restclient-cpp *.o file
set(RESTCLIENTCPP ${EXTERNAL_INSTALL_LOCATION}/lib/librestclient-cpp.a)


# Define an executable
add_executable(${PROJECT_NAME} ${SRC_FILES}) 

set_target_properties(${PROJECT_NAME} PROPERTIES 
                      VERSION ${PROJECT_VER}
                      SOVERSION ${PROJECT_APIVER})

add_dependencies(${PROJECT_NAME} restclient-cpp)

# Define the libraries this project depends upon
target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES} faxparser smsGateway ${RESTCLIENTCPP})

install(TARGETS ${PROJECT_NAME} DESTINATION alarmPublisher)

