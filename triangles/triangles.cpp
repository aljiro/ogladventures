// Include standard headers
#include <stdio.h>
#include <stdlib.h>
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.hpp>

using namespace glm;

const int numVAOs = 2;
const int numBuffers = 2;
GLuint VAOs[numVAOs];
GLuint buffers[numBuffers];
GLFWwindow* window;
GLuint MatrixId;
GLuint programId;
mat4 mvp;
int width = 1024;
int height = 768;

// Colors
static GLfloat g_color_buffer_data[] = {
    1.0f, 0.0f, 0.0f,
    0.7f, 0.3f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.3f, 0.7f,
    0.0f, 1.0f, 1.0f
};

// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   -1.0f, 1.0f, 0.0f,
    0.8f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -0.8f, 1.0f, 0.0f
};

float sigmas[6*3];
float t;

void transformations( GLuint programId ){
    mat4 P = glm::perspective( glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f );
    mat4 V = glm::lookAt( vec3(1, 1, 3), vec3(0, 0, 0), vec3(0, 1, 0) );
    mat4 M = glm::mat4(0.1f);
    mvp = P*V*M;

    // Passing to opengl
    MatrixId = glGetUniformLocation( programId, "MVP" );
}

void init(){
    t = 0.0f;

    for( int i = 0; i < 6*3; i++ ){
        sigmas[i] = (float)rand()/(float)RAND_MAX;
    }

    glGenVertexArrays( 2, VAOs );
    glGenBuffers( 2, buffers );
    // Binding vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Binding color buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    programId = LoadShaders("SimpleVertexShader.vsh", "SimpleFragmentShader.fsh");
    glUseProgram( programId );

    glBindVertexArray( VAOs[0] );
    glBindVertexArray( VAOs[1] );

    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    glVertexAttribPointer(
       1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    transformations(programId);

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
}

void display(){
    for( int i = 0; i < 6*3; i+=2 ){
        g_color_buffer_data[i] = exp(-1*sigmas[i]*t);
    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(
       1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );


    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindVertexArray(VAOs[0]);
    glBindVertexArray(VAOs[1]);
    glUniformMatrix4fv( MatrixId, 1, GL_FALSE, &mvp[0][0] );
    glDrawArrays(GL_TRIANGLES, 0, 3*2); // Starting from vertex 0; 3 vertices total -> 1 triangle
    //glDisableVertexAttribArray(0);
    t += 0.01f;
}

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( width, height, "Playground", NULL, NULL);

    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true; // Needed for core profile

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    init();

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.0f, 0.3f, 0.4f, 0.0f);

    do{
        display();
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, buffers );
    glDeleteProgram(programId);
    glDeleteVertexArrays(1, VAOs );


    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

