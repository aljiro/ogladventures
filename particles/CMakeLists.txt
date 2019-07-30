cmake_minimum_required (VERSION 2.6)
project (particles)
add_executable(particles particles.cpp)

#add_subdirectory (lib)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(glew REQUIRED)

include_directories(
	${GLFW_INCLUDE}
	${GLM_INCLUDE}
	${INCLUDE_INCLUDE}
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	${GLM_LIBRARY}
	GLEW
)

target_link_libraries(particles
	${ALL_LIBS}
)

