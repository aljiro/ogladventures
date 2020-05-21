cmake_minimum_required(VERSION 3.5)

project(Spring)

find_package(glfw3 3.1 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(Spring spring_ogl.cpp)

target_link_libraries(Spring ${OPENGL_LIBRARY})
target_link_libraries(Spring glfw)


