/*
 * Soft Animats Simulator
 * v 1.0
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "utils.h"

#define numVAOs 1
#define numVBOs 2

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs], vbo[numVBOs];
float cameraX, cameraY, cameraZ;
float cX, cY, cZ, pX, pY, pZ;

GLuint mvLoc, prLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

void setupVertices(){
	float cubePositions[108] = {
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};

	float pyramidPositions[54] = {
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
	};

	glGenVertexArrays( numVAOs, vao );
	glBindVertexArray( vao[0] );
	// Vertex Buffers
	glGenBuffers( numVBOs, vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW );
}

void init( GLFWwindow* window ){
	renderingProgram = Utils::createShaderProgram("shaders/cube.vsh", "shaders/cube.fsh");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 25.0f;
	cX = 0.0f; cY = -2.0f; cZ = 0.0f;
	pX = 3.0f; pY = 0.0f; cZ = 0.0f;
	setupVertices();	
}


void display( GLFWwindow* window, double currentTime ){

	glClear( GL_DEPTH_BUFFER_BIT );
	// glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glUseProgram( renderingProgram );

	mvLoc = glGetUniformLocation( renderingProgram, "MV" );
	prLoc = glGetUniformLocation( renderingProgram, "P" );
	// Build perspective matrix
	glfwGetFramebufferSize( window, &width, &height );
	aspect = (float)width/(float)height;
	pMat = glm::perspective( 1.0472f, aspect, 0.1f, 1000.0f );
	// View and model matrices
	vMat = glm::translate( glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ) );
	
	mMat = glm::translate( glm::mat4(1.0f), glm::vec3(cX, cY, cZ) );	
	mvMat = vMat*mMat;
	
	// Copy matrices into uniform variables
	glUniformMatrix4fv( mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat) );
	glUniformMatrix4fv( prLoc, 1, GL_FALSE, glm::value_ptr(pMat) );
	//Associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( 0 );
	// Adjust settings
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glDrawArrays( GL_TRIANGLES, 0, 36 );

	// Draw pyramid
	mMat = glm::translate( glm::mat4(1.0f), glm::vec3(pX, pY, pZ) );
	mvMat = vMat*mMat;
	glUniformMatrix4fv( mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat) );
	glUniformMatrix4fv( prLoc, 1, GL_FALSE, glm::value_ptr(pMat) );
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( 0 );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glDrawArrays( GL_TRIANGLES, 0, 18 );
}

int main( void ){

	if( !glfwInit() ){ exit(EXIT_FAILURE); }

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	GLFWwindow* window = glfwCreateWindow( 600, 600, "A cube!", NULL, NULL );
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
