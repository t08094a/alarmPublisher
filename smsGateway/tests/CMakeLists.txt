cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set options for this project
set (PROJECT_NAME "smsGateway_test" CXX)
project(${PROJECT_NAME})

message(STATUS "============================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "============================")

set (PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/../../alarmPublisher/include)
set (PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR})

set(CMAKE_BUILD_TYPE Debug)
message( STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}" )

add_definitions(-std=c++11)
add_definitions(-DBOOST_LOG_DYN_LINK)

enable_testing()

# Load settings for the external project "Boost".
# The components "system" and "filesystem" from "Boost" version 1.58.0 or
# greater are required.
find_package( Boost 1.58.0 REQUIRED COMPONENTS log regex date_time system filesystem unit_test_framework )
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

# add_executable(${PROJECT_NAME}_SmsTradeGateway TestRunner.cpp SmsTradeGatewayTest.cpp ${SMSTRADE_SRC_FILES} ${PROJECT_SOURCE_DIR}/../../alarmPublisher/src/ConfigReader.cpp)
# target_link_libraries(${PROJECT_NAME}_SmsTradeGateway PRIVATE smsGateway ${Boost_LIBRARIES} ${GSOAP_SSL_LIBRARIES} gsoapssl++)
# add_test(NAME SmsTradeGateway COMMAND ${PROJECT_NAME}_SmsTradeGateway)

add_executable(${PROJECT_NAME}_CmGateway TestRunner.cpp CmGatewayTest.cpp ${PROJECT_SOURCE_DIR}/../../alarmPublisher/src/ConfigReader.cpp)
target_link_libraries(${PROJECT_NAME}_CmGateway PRIVATE smsGateway ${Boost_LIBRARIES})
add_test(NAME CmGateway COMMAND ${PROJECT_NAME}_CmGateway)

add_executable(${PROJECT_NAME}_ProwlGateway TestRunner.cpp ProwlGatewayTest.cpp ${PROJECT_SOURCE_DIR}/../../alarmPublisher/src/ConfigReader.cpp) 
target_link_libraries(${PROJECT_NAME}_ProwlGateway PRIVATE smsGateway ${Boost_LIBRARIES})
add_test(NAME ProwlGateway COMMAND ${PROJECT_NAME}_ProwlGateway)

add_executable(${PROJECT_NAME}_GatewayManager TestRunner.cpp GatewayManagerTest.cpp ${PROJECT_SOURCE_DIR}/../../alarmPublisher/src/ConfigReader.cpp)
target_link_libraries(${PROJECT_NAME}_GatewayManager PRIVATE smsGateway ${Boost_LIBRARIES})
add_test(NAME GatewayManager COMMAND ${PROJECT_NAME}_GatewayManager)

add_executable(${PROJECT_NAME}_UrlEncoder TestRunner.cpp UrlEncoderTest.cpp) 
target_link_libraries(${PROJECT_NAME}_UrlEncoder PRIVATE smsGateway ${Boost_LIBRARIES})
add_test(NAME UrlEncoder COMMAND ${PROJECT_NAME}_UrlEncoder)
