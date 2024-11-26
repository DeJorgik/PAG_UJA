cmake_minimum_required(VERSION 3.27)
project(PAG_1)

set(CMAKE_CXX_STANDARD 17)

file( GLOB IMGUI imgui/*cpp)

find_package(glfw3)
find_package(glm)

add_executable(PAG_1 glad/src/glad.c
        main.cpp
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
        Utilities/Model.cpp
        Utilities/Model.h
        imgui/imfilebrowser.h
        Utilities/Material.cpp
        Utilities/Material.h
        Utilities/Light.cpp
        Utilities/Light.h
)

target_include_directories(PAG_1 PUBLIC glad/include)

set(ASSIMP_BUILD_DLL ON CACHE BOOL "Build Assimp as a DLL")
add_subdirectory(assimp)  # Añadir la carpeta Assimp al proyecto como un subdirectorio
include_directories(${CMAKE_SOURCE_DIR}/assimp/include)  # Incluir los archivos de encabezado de Assimp
set(ASSIMP_DLL_PATH "${CMAKE_SOURCE_DIR}/external_libs")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ASSIMP_DLL_PATH}/libassimp-5d.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

target_link_libraries(PAG_1 glfw)
target_link_libraries(PAG_1 glm::glm)

target_link_libraries(PAG_1 assimp)
