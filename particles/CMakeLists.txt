cmake_minimum_required (VERSION 2.6)
project (Particles)
add_executable(Particles particles.cpp)

add_subdirectory (lib)

find_package(OpenGL REQUIRED)

include_directories(
	lib/glfw-3.1.2/include/
	lib/glm-0.9.7.1/
	lib/glew-1.13.0/include/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

target_link_libraries(Particles
	${ALL_LIBS}
)

