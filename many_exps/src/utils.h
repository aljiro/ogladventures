#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Utils{
public:
    static string readShaderSource( const char* path ){
        string content;
        ifstream fileStream( path, ios::in );
        string line = "";

        while( !fileStream.eof() ){
            getline( fileStream, line );
            content.append( line + "\n" );
        }


            fileStream.close();
            return content;
    }

    static void printShaderLog( GLuint shader ){
        int len = 0;
        int chWrittn = 0;
        char *log;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

        if( len > 0 ){
            log = (char *)malloc( len );
            glGetShaderInfoLog( shader, len, &chWrittn, log );
            cout << "Shader Info Log: " << log << endl;
            free( log );
        }
    }

    static void printProgramLog( int prog ){
        int len = 0;
        int chWrittn = 0;
        char *log;
        glGetProgramiv( prog, GL_INFO_LOG_LENGTH, &len );

        if( len > 0 ){
            log = (char *)malloc(len);
            glGetProgramInfoLog( prog, len, &chWrittn, log );
            cout << "Program Info Log: " << log << endl;
            free( log );
        }
    }

    static bool checkOpenGLError(){
        bool foundError = false;
        int glErr = glGetError();

        while( glErr != GL_NO_ERROR ){
            cout << "glError: " << glErr << endl;
            foundError = true;
            glErr = glGetError();
        }

        return foundError;
    }

    static GLuint createShaderProgram( const char* vn, const char* fn ){
        GLint vCompiled, fCompiled, linked;
        string vs = readShaderSource( vn );
        string fs = readShaderSource( fn );
        const char *vshaderSource = vs.c_str();
        const char *fshaderSource = fs.c_str();
        
        GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
        GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER );
        
        glShaderSource( vShader, 1, &vshaderSource, NULL );
        glShaderSource( fShader, 1, &fshaderSource, NULL );
        glCompileShader( vShader );
        glGetShaderiv( vShader, GL_COMPILE_STATUS, &vCompiled );

        if( checkOpenGLError() || vCompiled != 1 ){
            printShaderLog( vShader );
        }

        glCompileShader( fShader );
        glGetShaderiv( vShader, GL_COMPILE_STATUS, &fCompiled );

        if( checkOpenGLError() || fCompiled != 1 ){
            printShaderLog( fShader );
        }

        GLuint vfProgram = glCreateProgram();
        glAttachShader( vfProgram, vShader );
        glAttachShader( vfProgram, fShader );
        glLinkProgram( vfProgram );
        glGetProgramiv( vfProgram, GL_LINK_STATUS, &linked );

        if( checkOpenGLError() || linked != 1 ){
            printProgramLog( vfProgram );
        }

        return vfProgram;
    }

};