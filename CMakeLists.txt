cmake_minimum_required(VERSION 3.22)

project(BSPViewer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("thirdparty")

add_executable(BSPViewer src/main.cpp)
target_include_directories(BSPViewer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(BSPViewer PUBLIC glad glfw ImGUI)
target_compile_options(BSPViewer PRIVATE -Wall -Wextra -Wpedantic)
