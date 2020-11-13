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
float cX, cY, cZ;

GLuint vLoc, mLoc, prLoc, tfLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

void setupVertices(){
	float vertexPositions[108] = {
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

	glGenVertexArrays( numVAOs, vao );
	glBindVertexArray( vao[0] );
	glGenBuffers( numVBOs, vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW );
}

void init( GLFWwindow* window ){
	renderingProgram = Utils::createShaderProgram("shaders/cube_instanced.vsh", "shaders/cube.fsh");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 219.0f;
	cX = 0.0f; cY = 0.0f; cZ = 0.0f;
	setupVertices();	
}



void display( GLFWwindow* window, double currentTime ){
	float timeFactor;
	timeFactor = (float)currentTime;
	glClear( GL_DEPTH_BUFFER_BIT );
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glUseProgram( renderingProgram );

	vLoc = glGetUniformLocation( renderingProgram, "V" );
	mLoc = glGetUniformLocation( renderingProgram, "M" );
	prLoc = glGetUniformLocation( renderingProgram, "P" );
	tfLoc = glGetUniformLocation(renderingProgram, "tf");
	// Build perspective matrix
	glfwGetFramebufferSize( window, &width, &height );
	aspect = (float)width/(float)height;
	pMat = glm::perspective( 1.0472f, aspect, 0.1f, 1000.0f );
	// View and model matrices
	vMat = glm::translate( glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ) );
	mMat = glm::translate( glm::mat4(1.0f), glm::vec3(cX, cY, cZ) );
	
	// Copy matrices into uniform variables
	glUniformMatrix4fv( vLoc, 1, GL_FALSE, glm::value_ptr(vMat) );
	glUniformMatrix4fv( mLoc, 1, GL_FALSE, glm::value_ptr(mMat) );
	glUniformMatrix4fv( prLoc, 1, GL_FALSE, glm::value_ptr(pMat) );	
	glUniform1f( tfLoc, timeFactor );
	//Associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( 0 );

	// Adjust settings
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glDrawArraysInstanced( GL_TRIANGLES, 0, 36, 100000 );
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
