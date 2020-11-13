/*
 * Soft Animats Simulator
 * v 1.0
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "utils.h"

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

using namespace std;



float x = 0.0f, y = 0.0f;
float vx = 0.01f, vy = 0.01f;

void init( GLFWwindow* window ){
	vx = (rand() % 10)/1000.0;
	vy = (rand() % 10)/1000.0;
	renderingProgram = Utils::createShaderProgram("shaders/ball.vsh", "shaders/ball.fsh");
	glGenVertexArrays( numVAOs, vao );
	glPointSize( 10.0f );
	glBindVertexArray( vao[0] );	
}


void display( GLFWwindow* window, double currentTime ){
	float s = sqrt(x*x + y*y)*10.0f;
	glPointSize( s );
	glClear( GL_DEPTH_BUFFER_BIT );
	// glClearColor( 0.0, 0.0, 0.0, 1.0 );
	// glClear( GL_COLOR_BUFFER_BIT );

	glUseProgram( renderingProgram );

	x += vx;
	y += vy;
	
	if( x > 1.0f || x < -1.0f) vx *= -1;
	if( y > 1.0f || y < -1.0f ) vy *= -1;

	GLuint xLoc = glGetUniformLocation( renderingProgram, "x" );
	GLuint yLoc = glGetUniformLocation( renderingProgram, "y" );
	glProgramUniform1f( renderingProgram, xLoc, x );
	glProgramUniform1f( renderingProgram, yLoc, y );
	glDrawArrays( GL_POINTS, 0, 1 );
}

int main( void ){

	srand(time(NULL));

	if( !glfwInit() ){ exit(EXIT_FAILURE); }

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	GLFWwindow* window = glfwCreateWindow( 600, 600, "Shape matching playground", NULL, NULL );
	glfwMakeContextCurrent( window );

	if( glewInit() != GLEW_OK ){ exit(EXIT_FAILURE); }

	glfwSwapInterval( 1 );
	init( window );

	while( !glfwWindowShouldClose( window ) ){
		display( window, glfwGetTime() );
		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwDestroyWindow( window );
	glfwTerminate();
	exit( EXIT_SUCCESS );
}