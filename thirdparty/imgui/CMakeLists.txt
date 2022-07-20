project(ImGUI LANGUAGES CXX)

add_library(ImGUI imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp backends/imgui_impl_glfw.cpp backends/imgui_impl_opengl3.cpp)
target_include_directories(ImGUI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(ImGUI PUBLIC glfw)