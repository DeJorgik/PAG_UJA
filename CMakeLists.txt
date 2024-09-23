cmake_minimum_required(VERSION 3.27)
project(PAG_1)

set(CMAKE_CXX_STANDARD 17)

file( GLOB IMGUI imgui/*cpp)

add_executable(PAG_1 main.cpp
        Utilities/Renderer.cpp
        Utilities/Renderer.h
        Model.cpp
        Model.h
        Utilities/GUI.cpp
        Utilities/GUI.h
        ${IMGUI}
)

find_package(glfw3)
find_package(glad)

target_link_libraries(PAG_1 glad::glad)
target_link_libraries(PAG_1 glfw)

