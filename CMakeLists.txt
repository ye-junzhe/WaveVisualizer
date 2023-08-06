# Generate the COMPILE COMMANDS, thus lsp could work
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.10)

# The name of this project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

# Set standard
set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)

add_executable(visualizer src/WaveVisualizer.c src/plug.c)

target_compile_options(visualizer PRIVATE -Wall -Wextra)

target_link_libraries(visualizer PRIVATE raylib dl m pthread)
