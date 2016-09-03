cmake_minimum_required (VERSION 3.6)
project ("voxeltronik")
file (GLOB_RECURSE SOURCES "src/*.cpp")
include_directories("include")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
add_executable(vtk ${SOURCES})

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
target_link_libraries(vtk SDL2 GL GLEW GLU noise ${CMAKE_THREAD_LIBS_INIT})

