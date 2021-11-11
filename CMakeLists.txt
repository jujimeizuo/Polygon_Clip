cmake_minimum_required(VERSION 3.20)
project(Polygon_clip)

include_directories(include)
link_directories(lib/x64)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_executable(Polygon_clip main.cpp)
target_link_libraries(Polygon_clip libopengl32.a  libfreeglut.a glu32.dll)