cmake_minimum_required(VERSION 3.5)

project(Ogl_adventures)

find_package(glfw3 3.1 REQUIRED)
find_package(OpenGL REQUIRED)

# Spring 
add_executable(Spring spring_ogl.cpp)

target_link_libraries(Spring ${OPENGL_LIBRARY})
target_link_libraries(Spring glfw)

# Exercise 1
add_executable(e1_tutorial e1_tutorial_ogl.cpp)

target_link_libraries(e1_tutorial ${OPENGL_LIBRARY})
target_link_libraries(e1_tutorial glfw)


