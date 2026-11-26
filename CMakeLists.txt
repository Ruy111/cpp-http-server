cmake_minimum_required(VERSION 3.16)

project(HttpServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(http_server
    src/main.cpp
    src/net/Server.cpp
)

target_include_directories(http_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(http_server PRIVATE
    -Wall
    -Wextra
    -g
)
