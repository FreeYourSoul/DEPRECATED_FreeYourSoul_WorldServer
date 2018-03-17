cmake_minimum_required(VERSION 3.3)

project(FreeSouls_World CXX)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lprotobuf -lboost_timer -lboost_system -lboost_thread -lboost_date_time -pthread")
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
        ${CMAKE_SOURCE_DIR}/cmake-build-debug/Protobuf
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue
        ${CMAKE_SOURCE_DIR}/FySMQ/include/bus
        ${CMAKE_SOURCE_DIR}/FySMemoryManager/include
        ${CMAKE_SOURCE_DIR}/Server/include/ws
        ${CMAKE_SOURCE_DIR}/Server/include/ws/tmx
        ${CMAKE_SOURCE_DIR}/Server/include/network
        ${CMAKE_SOURCE_DIR}/Server/include/game Utils/src)

set(SOURCE_FILES_MM)

set(SOURCE_FILES_MQ
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue/LockFreeQueue.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue/QueueContainer.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue/LockingQueue.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/bus/BusListener.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/bus/FysBus.hh)

set(SOURCE_FILES_SRV
        ${CMAKE_SOURCE_DIR}/Server/src/ws/WorldServer.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/ws/WorldServer.hh
        ${CMAKE_SOURCE_DIR}/Server/src/ws/Context.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/ws/Context.hh
        ${CMAKE_SOURCE_DIR}/Server/src/network/SessionManager.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/network/SessionManager.hh
        ${CMAKE_SOURCE_DIR}/Server/src/network/TcpConnection.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/network/TcpConnection.hh
        ${CMAKE_SOURCE_DIR}/Server/src/ws/GameServerInstance.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/ws/GameServerInstance.hh
        ${CMAKE_SOURCE_DIR}/Server/src/ws/Authenticator.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/ws/Authenticator.hh
        ${CMAKE_SOURCE_DIR}/Server/src/ws/tmx/MapBuilder.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/ws/tmx/MapBuilder.hh
        ${CMAKE_SOURCE_DIR}/Server/src/ws/tmx/Map.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/ws/tmx/Map.hh
        ${CMAKE_SOURCE_DIR}/Server/src/network/ClusterManager.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/network/ClusterManager.hh
        ${CMAKE_SOURCE_DIR}/Server/src/network/PlayerManager.cpp
        ${CMAKE_SOURCE_DIR}/Server/include/network/PlayerManager.hh)

set(SOURCE_FILES_UTILS
        ${CMAKE_SOURCE_DIR}/Utils/src/CheckVars.hh
        ${CMAKE_SOURCE_DIR}/Utils/src/TokenGenerator.cpp
        ${CMAKE_SOURCE_DIR}/Utils/src/TokenGenerator.hh)

set(SOURCE_FILES_ALL
        ${SOURCE_PROTOBUF}
        ${SOURCE_FILES_UTILS}
        ${SOURCE_FILES_MQ}
        ${SOURCE_FILES_MM}
        ${SOURCE_FILES_SRV})

set(SOURCE_FILES
        ${SOURCE_FILES_ALL}
        ${CMAKE_SOURCE_DIR}/Server/src/main.cpp
        )

set(SOURCE_FILES_TEST
        ${SOURCE_FILES_ALL}
        ${CMAKE_SOURCE_DIR}/FySMQ/test/TestUnitMQ.cpp
        ${CMAKE_SOURCE_DIR}/Utils/test/TestCheckVar.cpp
        )

## Test executable
add_executable(UnitTests ${SOURCE_FILES_TEST})
target_link_libraries(UnitTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(UnitTests UnitTests)

### Server ###
add_executable(FreeSouls_World ${SOURCE_FILES})
target_link_libraries(FreeSouls_World proto ${TMXLITE_LIBRARIES})