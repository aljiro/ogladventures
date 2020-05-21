#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

GLFWwindow * window;

static void error_callback( int errno, const char* description ){
	std::cerr << "Error: " << description << std::endl;
}

static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ){
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, 1 );
}


int main( int n, char **args ){
	glfwSetErrorCallback( error_callback );

	if( !glfwInit() ){
		std::cerr << "There was an error initializing glfw" << std::endl;
		return -1;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	window = glfwCreateWindow( 640, 680, "Nice colors", NULL, NULL );

	if( !window ){
		std::cerr << "Error ocurred when creating window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);

	}

	glfwMakeContextCurrent( window );
	glfwSetKeyCallback( window, key_callback );

	int width, height;
	glfwGetFramebufferSize( window, &width, &height );
	glViewport( 0, 0, width, height );

	while( !glfwWindowShouldClose( window ) ){
		// Do opengl magic

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}