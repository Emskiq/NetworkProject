cmake_minimum_required(VERSION 3.23)
project(NetCommon)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio
        GIT_TAG asio-1-14-0
        UPDATE_DISCONNECTED 1
)

FetchContent_Populate(asio)
include_directories(${asio_SOURCE_DIR}/asio/include/)

link_libraries(ws2_32 wsock32)

add_library(NetCommon library.cpp net_message.h net_common.h olc_net.h net_tsqueue.h net_connection.h net_server.h net_client.h)

add_executable(NetCommonExecutable SimpleClient.cpp)
target_link_libraries(NetCommonExecutable NetCommon)
