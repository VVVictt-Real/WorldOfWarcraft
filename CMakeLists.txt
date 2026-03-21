cmake_minimum_required(VERSION 3.10)

project(WorldOfWarcraft)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SRC_LIST "src/*.cpp")

list(APPEND SRC_LIST "main.cpp")

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_executable(WorldOfWarcraft ${SRC_LIST})
