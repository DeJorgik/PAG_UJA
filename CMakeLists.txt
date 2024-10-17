cmake_minimum_required(VERSION 3.27)
project(PAG_1)

set(CMAKE_CXX_STANDARD 17)

file( GLOB IMGUI imgui/*cpp)

add_executable(PAG_1 main.cpp
        Utilities/Renderer.cpp
        Utilities/Renderer.h
        Utilities/GUI.cpp
        Utilities/GUI.h
        ${IMGUI}
        Utilities/ShaderProgram.cpp
        Utilities/ShaderProgram.h
        Utilities/ShaderObject.cpp
        Utilities/ShaderObject.h
        Utilities/Camera.cpp
        Utilities/Camera.h
)

find_package(glfw3)
find_package(glad)
find_package(glm)


target_link_libraries(PAG_1 glad::glad)
target_link_libraries(PAG_1 glfw)
target_link_libraries(PAG_1 glm::glm)