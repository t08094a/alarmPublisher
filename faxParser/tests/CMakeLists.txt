cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/CMakeModules")

# Set options for this project
set (PROJECT_NAME "faxparser_tests" CXX)
project(${PROJECT_NAME})

message(STATUS "============================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "============================")

set (PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR})
set (PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR})

add_definitions(-std=c++11)

enable_testing()

set(Boost_USE_STATIC_LIBS ON)

# Load settings for the external project "Boost".
# The components "system" and "filesystem" from "Boost" version 1.58.0 or
# greater are required.
find_package( Boost 1.58.0 REQUIRED COMPONENTS system filesystem regex date_time unit_test_framework )
if (Boost_FOUND)
    # From the offical documentation:
    # Add include directories to the build. [...] If the SYSTEM option is given,
    # the compiler will be told the directories are meant as system include
    # directories on some platforms (signalling this setting might achieve effects
    # such as the compiler skipping warnings [...])."
    include_directories (SYSTEM ${Boost_INCLUDE_DIR})
    
    # From the offical documentation:
    # "Specify directories in which the linker will look for libraries. [...] Note
    # that this command is rarely necessary. Library locations returned by
    # find_package() and find_library() are absolute paths. Pass these absolute
    # library file paths directly to the target_link_libraries() command. CMake
    # will ensure the linker finds them."
    link_directories (${Boost_LIBRARY_DIRS})

    message(STATUS "Boost libs: ${Boost_LIBRARIES}")
endif ()

add_executable(${PROJECT_NAME}_ParserUtility TestRunner.cpp ParserUtilityTest.cpp ) 
target_link_libraries(${PROJECT_NAME}_ParserUtility PRIVATE faxparser ${Boost_LIBRARIES})
add_test(NAME ParserUtility COMMAND ${PROJECT_NAME}_ParserUtility)

add_executable(${PROJECT_NAME}_PropertyLocation TestRunner.cpp PropertyLocationTest.cpp ) 
target_link_libraries(${PROJECT_NAME}_PropertyLocation PRIVATE faxparser ${Boost_LIBRARIES})
add_test(NAME PropertyLocation COMMAND ${PROJECT_NAME}_PropertyLocation)

add_executable(${PROJECT_NAME}_Operation TestRunner.cpp OperationTest.cpp ) 
target_link_libraries(${PROJECT_NAME}_Operation PRIVATE faxparser ${Boost_LIBRARIES})
add_test(NAME Operation COMMAND ${PROJECT_NAME}_Operation)

add_executable(${PROJECT_NAME}_OperationKeywords TestRunner.cpp OperationKeywordsTest.cpp ) 
target_link_libraries(${PROJECT_NAME}_OperationKeywords PRIVATE faxparser ${Boost_LIBRARIES})
add_test(NAME OperationKeywords COMMAND ${PROJECT_NAME}_OperationKeywords)

add_executable(${PROJECT_NAME}_OperationResource TestRunner.cpp OperationResourceTest.cpp ) 
target_link_libraries(${PROJECT_NAME}_OperationResource PRIVATE faxparser ${Boost_LIBRARIES})
add_test(NAME OperationResource COMMAND ${PROJECT_NAME}_OperationResource)

add_executable(${PROJECT_NAME}_IlsAnsbach TestRunner.cpp IlsAnsbachTest.cpp) 
target_link_libraries(${PROJECT_NAME}_IlsAnsbach PRIVATE faxparser ${Boost_LIBRARIES})
add_test(NAME IlsAnsbach COMMAND ${PROJECT_NAME}_IlsAnsbach)

add_executable(${PROJECT_NAME}_GaussKrueger TestRunner.cpp GaussKruegerCoordinateTest.cpp) 
target_link_libraries(${PROJECT_NAME}_GaussKrueger PRIVATE faxparser ${Boost_LIBRARIES})
add_test(NAME GaussKrueger COMMAND ${PROJECT_NAME}_GaussKrueger)
