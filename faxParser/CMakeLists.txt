cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/CMakeModules/")

# Set options for this project
set(PROJECT_NAME "faxparser" CXX)
project(${PROJECT_NAME})

message(STATUS "============================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "============================")

set(PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

include_directories (${PROJECT_SOURCE_DECLARATION_DIRECTORY})
include_directories(tests)

# find all files in ./src/ and ./include/
aux_source_directory(src SRC_FILES)
aux_source_directory(include HEADER_FILES)

add_library(${PROJECT_NAME} ${SRC_FILES})
message(STATUS "Source files: ${SRC_FILES}")

add_subdirectory(tests)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Load settings for the external project "Boost".
# The components "system" and "filesystem" from "Boost" version 1.55.0 or
# greater are required.
find_package( Boost 1.55.0 REQUIRED COMPONENTS log regex date_time )
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


#exe install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
# install(TARGETS ${PROJECT_NAME} DESTINATION lib)
# install(FILES ${HEADER_FILES} DESTINATION include)

# 'make install' to the correct location
install(TARGETS ${PROJECT_NAME}
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(FILES ${HEADER_FILES} DESTINATION include/${PROJECT_NAME})

target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})

set(faxparser_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})