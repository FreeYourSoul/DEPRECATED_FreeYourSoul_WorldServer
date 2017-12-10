cmake_minimum_required(VERSION 3.3)

project(FreeSouls CXX)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lprotobuf -lboost_timer -lboost_system -lboost_thread")
#SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_STANDARD 17)

if ( UNIX )
    set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wno-deprecated-declarations")
    message( STATUS Set Warnings flag : ${CMAKE_CXX_FLAGS})
endif()

enable_testing()

add_subdirectory( Protobuff )

include_directories(
        cmake-build-debug/Protobuff
        FySMQ/include/queue
        FySMQ/include/bus
        FySMemoryManager/include
        Server/include/ws
        Server/include/network
        Server/include/game Utils/src)

set(SOURCE_FILES_MM)

set(SOURCE_PROTOBUF
        cmake-build-debug/Protobuff/FySMessage.pb.cc
        cmake-build-debug/Protobuff/FySAuthenticationLoginMessage.pb.cc
        cmake-build-debug/Protobuff/FySAuthenticationResponse.pb.cc
)

set(SOURCE_FILES_MQ
        FySMQ/include/queue/LockFreeQueue.hh
        FySMQ/include/queue/QueueContainer.hh
        FySMQ/include/queue/LockingQueue.hh
        FySMQ/include/bus/BusListener.hh
        FySMQ/include/bus/FysBus.hh)

set(SOURCE_FILES_SRV
        Server/src/gateway/WorldServer.cpp
        Server/include/ws/WorldServer.hh
        Server/src/gateway/Context.cpp
        Server/include/ws/Context.hh
        Server/src/network/SessionManager.cpp
        Server/include/network/SessionManager.hh
        Server/src/network/TcpConnection.cpp
        Server/include/network/TcpConnection.hh
        Server/src/gateway/GameServerInstance.cpp
        Server/include/ws/GameServerInstance.hh
        Server/src/gateway/Authenticator.cpp
        Server/include/ws/Authenticator.hh
)

set(SOURCE_FILES_UTILS Utils/src/CheckVars.hh Utils/src/TokenGenerator.cpp Utils/src/TokenGenerator.hh)

set(SOURCE_FILES_ALL
        ${SOURCE_PROTOBUF}
        ${SOURCE_FILES_UTILS}
        ${SOURCE_FILES_MQ}
        ${SOURCE_FILES_MM}
        ${SOURCE_FILES_SRV})

set(SOURCE_FILES
        ${SOURCE_FILES_ALL}
        Server/src/main.cpp)

set(SOURCE_FILES_TEST
        ${SOURCE_FILES_ALL}
        FySMQ/test/TestUnitMQ.cpp
        Utils/test/TestCheckVar.cpp
        FySMQ/test/FysBusTest.hh)

### Server ###
add_executable(FreeSouls ${SOURCE_FILES})
target_link_libraries(FreeSouls proto )

# link against dynamic libraries
add_definitions(-DBOOST_ALL_DYN_LINK)

## Test executable
#find_package(TMXLITE REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(UnitTests ${SOURCE_FILES_TEST})
target_link_libraries(UnitTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(UnitTests UnitTests)