#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

static void error_callback( int num, const char* description ){
	std::cerr << "Error: " << description << std::endl;
}

int main( int nargs, char ** args ){

	glfwSetErrorCallback(error_callback);

	if( !glfwInit() )
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow( 800, 600, "Example 1", NULL, NULL );

	if( !window ){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent( window );

	while( !glfwWindowShouldClose( window ) ){
		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}