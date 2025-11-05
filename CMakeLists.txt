cmake_minimum_required(VERSION 3.10)
project(ImageModifier CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(image-modifier
    src/main.cpp
)
