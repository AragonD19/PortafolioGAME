cmake_minimum_required(VERSION 4.0)

project(GAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

# Fuentes del proyecto (GLOB_RECURSE ya captura subdirs como src/scenes/ y src/editor/)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Fuentes de ImGui y rlImGui
file(GLOB IMGUI_SOURCES "${PROJECT_SOURCE_DIR}/external/imgui/*.cpp")
file(GLOB RLIMGUI_SOURCES "${PROJECT_SOURCE_DIR}/external/rlImGui/rlImGui.cpp")

# Ejecutable
add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES} ${IMGUI_SOURCES} ${RLIMGUI_SOURCES})

# Includes (agrega include/scenes/)
target_include_directories(${PROJECT_NAME}
  PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/scenes  # Nuevo
    ${PROJECT_SOURCE_DIR}/external/imgui
    ${PROJECT_SOURCE_DIR}/external/rlImGui
  PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
  raylib
)

# Copia assets al build dir (para paths relativos)
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)