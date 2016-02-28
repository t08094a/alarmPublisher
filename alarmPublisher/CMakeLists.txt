cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set options for this project
set(PROJECT_NAME "alarmPublisher" CXX)
project(${PROJECT_NAME})

message(STATUS "============================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "============================")

# set(PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
# set(PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

# include_directories (${PROJECT_SOURCE_DECLARATION_DIRECTORY})
# include_directories(tests)

# find all source files in ./src/
aux_source_directory(src SRC_FILES)
# aux_source_directory(include HEADER_FILES)

message(STATUS "Source files: ${SRC_FILES}")

#add_subdirectory(tests)

# Add faxparser to #include search path
include_directories(${faxparser_SOURCE_DIR})

# Define an executable
add_executable(${PROJECT_NAME} ${SRC_FILES}) 

# Define the libraries this project depends upon
target_link_libraries (${PROJECT_NAME} faxparser)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
