cmake_minimum_required (VERSION 2.6)

project (many_exps)

# Required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(glew REQUIRED)
find_package(glew REQUIRED)
find_package(Armadillo REQUIRED)

include_directories(
	${GLFW_INCLUDE}
	${GLM_INCLUDE}
	${INCLUDE_INCLUDE}
	${ARMADILLO_INCLUDE_DIR}
	"${PROJECT_SOURCE_DIR}/include"
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	${GLM_LIBRARY}
	GLEW
	armadillo
	openblas
	lapack
)


file(GLOB SOURCES
    src/*.cpp
)

set(targets ball cube cube_instanced cube_pyramid)

foreach(X IN LISTS targets)
	message("Configuring ${X}")
	add_executable(${X} src/${X}.cpp)
	target_link_libraries(${X} ${ALL_LIBS})
	add_custom_command( TARGET ${X} POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders" 
					"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/shaders")
endforeach()

message("Copying the shaders from")
message("${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
message("to")
message("${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/")

