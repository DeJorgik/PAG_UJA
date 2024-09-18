cmake_minimum_required(VERSION 3.27)
project(PAG_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(PAG_1 main.cpp
        Renderer.cpp
        Renderer.h
        Model.cpp
        Model.h
)

find_package(glfw3)
find_package(glad)

target_link_libraries(PAG_1 glad::glad)
target_link_libraries(PAG_1 glfw)