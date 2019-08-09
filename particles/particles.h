// Particles header

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.hpp"

using namespace std;
using namespace glm;


class Particle{
public:
	float m;
	GLfloat x[3];
	GLfloat v[3];
	GLfloat f[3];
	GLfloat life;

	Particle():m(1.0f), life(0.0f){
		this->x[0] = 0.0f;
		this->x[1] = 0.0f;
		this->x[2] = 0.0f;
		this->v[0] = 0.0f;
		this->v[1] = 0.0f;
		this->v[2] = 0.0f;
		this->f[0] = 0.0f;
		this->f[1] = 0.0f;
		this->f[2] = 0.0f;
	}
};

class ParticleSystem;

class ForceObject{
private:
	ForceObject *nextObject;
public:
	ForceObject( ForceObject *fo );
	ForceObject *next();
	
	void applyForce( ParticleSystem *ps );
	// Abstract functions
	virtual void apply( ParticleSystem *ps ) = 0;
};

// Concrete force objects
class GravityForce: public ForceObject{
public:
	GravityForce( ForceObject *fo );
	void apply( ParticleSystem *ps );

};

class DragForce: public ForceObject{
private:
	float kd;

public:
	DragForce( ForceObject *fo );
	void apply( ParticleSystem *ps );
};

// Particle system

class ParticleSystem{

public:
	vector<Particle *> particles;
	ForceObject *fos;
	int n;
	float t;

	ParticleSystem( int n );

	void init();

	int particleDims();

	void getState( float *dst );

	void setState( float *src );

	void clearParticles();

	void reSpawn();

	void computeForces();

	void derivative( float *dst );

};

// Solver
class Solver{
private:
	float delta;
public:
	Solver();

	void scaleVector( ParticleSystem *ps, float *dst, float delta );

	void add( ParticleSystem *ps, float *dst, float *state, float *target );

	void step( ParticleSystem *ps );
};

// Simulation
class Simulation{
private:
	GLFWwindow* window;
	GLuint shaderId;
	GLuint offsetId;
	GLuint colorId;
	GLuint VAO;
	GLuint VBO;
	ParticleSystem *ps;
	int n;

public:

	Simulation( int n );

	int init();

	void setup();
	// Drawing routines	

	void draw( ParticleSystem *ps );

	int run();

};
