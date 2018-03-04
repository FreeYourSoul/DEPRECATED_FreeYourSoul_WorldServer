cmake_minimum_required(VERSION 3.3)

project(FreeSouls_World CXX)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lprotobuf -lboost_timer -lboost_system -lboost_thread -lboost_date_time")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_STANDARD 17)

if ( UNIX )
    set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wno-deprecated-declarations")
    message( STATUS Set Warnings flag : ${CMAKE_CXX_FLAGS})
endif()

enable_testing()

add_subdirectory(Protobuf)

include_directories(
        ${TMXLITE_INCLUDE_DIR}
        cmake-build-debug/Protobuf
        FySMQ/include/queue
        FySMQ/include/bus
        FySMemoryManager/include
        Server/include/ws
        Server/include/ws/tmx
        Server/include/network
        Server/include/game Utils/src)

set(SOURCE_FILES_MM)

set(SOURCE_PROTOBUF
        cmake-build-debug/Protobuf/FySMessage.pb.cc
        cmake-build-debug/Protobuf/FySAuthenticationLoginMessage.pb.cc
        cmake-build-debug/Protobuf/FySAuthenticationResponse.pb.cc
)

set(SOURCE_FILES_MQ
        FySMQ/include/queue/LockFreeQueue.hh
        FySMQ/include/queue/QueueContainer.hh
        FySMQ/include/queue/LockingQueue.hh
        FySMQ/include/bus/BusListener.hh
        FySMQ/include/bus/FysBus.hh)

set(SOURCE_FILES_SRV
        Server/src/ws/WorldServer.cpp
        Server/include/ws/WorldServer.hh
        Server/src/ws/Context.cpp
        Server/include/ws/Context.hh
        Server/src/network/SessionManager.cpp
        Server/include/network/SessionManager.hh
        Server/src/network/TcpConnection.cpp
        Server/include/network/TcpConnection.hh
        Server/src/ws/GameServerInstance.cpp
        Server/include/ws/GameServerInstance.hh
        Server/src/ws/Authenticator.cpp
        Server/include/ws/Authenticator.hh
        Server/src/ws/tmx/MapBuilder.cpp
        Server/include/ws/tmx/MapBuilder.hh
        Server/src/ws/tmx/Map.cpp
        Server/include/ws/tmx/Map.hh
        Server/src/network/ClusterManager.cpp
        Server/include/network/ClusterManager.hh)

set(SOURCE_FILES_UTILS Utils/src/CheckVars.hh Utils/src/TokenGenerator.cpp Utils/src/TokenGenerator.hh)

set(SOURCE_FILES_ALL
        ${SOURCE_PROTOBUF}
        ${SOURCE_FILES_UTILS}
        ${SOURCE_FILES_MQ}
        ${SOURCE_FILES_MM}
        ${SOURCE_FILES_SRV})

set(SOURCE_FILES
        ${SOURCE_FILES_ALL})

set(SOURCE_FILES_TEST
        ${SOURCE_FILES_ALL}
        FySMQ/test/TestUnitMQ.cpp
        Utils/test/TestCheckVar.cpp
        )

### Server ###
add_executable(FreeSouls_World ${SOURCE_FILES} Server/src/main.cpp)
target_link_libraries(FreeSouls_World proto ${TMXLITE_LIBRARIES})

# link against dynamic libraries
add_definitions(-DBOOST_ALL_DYN_LINK)

## Test executable
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(UnitTests ${SOURCE_FILES_TEST})
target_link_libraries(UnitTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(UnitTests UnitTests)