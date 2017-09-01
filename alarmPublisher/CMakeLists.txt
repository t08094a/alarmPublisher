cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set options for this project
set(PROJECT_NAME "alarmPublisher" CXX)
project(${PROJECT_NAME})

message(STATUS "============================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "============================")

SET(PROJECT_VER_MAJOR 1)
SET(PROJECT_VER_MINOR 0)
SET(PROJECT_VER_PATCH 0)
SET(PROJECT_VER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
SET(PROJECT_APIVER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}")

set(PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

include_directories (${PROJECT_SOURCE_DECLARATION_DIRECTORY})
include_directories(tests)

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

    message(STATUS "Boost libs: ${Boost_LIBRARIES}")
    
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
# aux_source_directory(include HEADER_FILES)

message(STATUS "Source files: ${SRC_FILES}")

#add_subdirectory(tests)

# Add faxparser to #include search path
include_directories(${faxparser_SOURCE_DIR})

# Define an executable
add_executable(${PROJECT_NAME} ${SRC_FILES}) 

set_target_properties(${PROJECT_NAME} PROPERTIES 
                      VERSION ${PROJECT_VER}
                      SOVERSION ${PROJECT_APIVER})

# Define the libraries this project depends upon
target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES} faxparser smsGateway)

install(TARGETS ${PROJECT_NAME} DESTINATION alarmPublisher)
