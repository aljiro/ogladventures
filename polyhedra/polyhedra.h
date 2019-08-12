// Implementation of regular polyhedra in opengl
#include <GL/glew>
#include <GLFW/glfw3>
#include <glm/glm.hpp>
#include <iostream>

class Polyhedron{
	GLfloat vertices[8*3];
public:
	Polyhedron();
	void draw();
};

class Simulation{
public:
	void init();
	void draw();
	void run();
};

