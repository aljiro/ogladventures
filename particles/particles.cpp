#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

typedef struct{
	float m;
	float *x;
	float *v;
	float *f;
} *Particle;

class ForceObject{
private:
	ForceObject *nextObject;
public:
	ForceObject( ForceObject *fo ){
		this->nextObject = fo;
	}

	ForceObject *next(){
		return this->nextObject;
	}
	
	void applyForce( ParticleSystem *ps ){
		this->apply( ps );

		if( this->nextObject == NULL ) this->next()->applyForce( ps );
	}
	// Abstract functions
	virtual void apply( ParticleSystem *ps );
};

// Concrete force objects
class GravityForce: ForceObject{
	void apply( ParticleSystem *ps ){
		for( int i = 0; i < ps->n; i++ ){
			ps[i]->p->f[0] = 0.0;
			ps[i]->p->f[1] = 0.0;
			ps[i]->p->f[2] = -10.0;
		}	
	}

}

class DragForce: ForceObject{
private:
	float kd;

public:
	DragForce(){
		this->kd = 0.01;
	}

	void apply( ParticleSystem *ps ){
		for( int i = 0; i < ps->n; i++ ){
			Particle p = ps[i];
			p->f[0] = this->kd*p->v[0];
			p->f[1] = this->kd*p->v[1];
			p->f[2] = this->kd*p->v[2];
		}	
	}
};

// Particle system
class ParticleSystem{

public:
	Particle *p;
	ForceObject *fos;
	int n;
	float t;

	ParticleSystem( int n ) : n(10){
		fos = (ForceObject) new GravityForce( new DragForce(NULL) );
		this->n = n;
		t = 0.0;
	}

	int particleDims(){
		return ( 6 * this->n );
	}

	void getState( float *dst ){
		for( int i = 0; i < this->n; i++ ){
			*(dst++) = this->p[i]->x[0];
			*(dst++) = this->p[i]->x[1];
			*(dst++) = this->p[i]->x[2];
			*(dst++) = this->p[i]->v[0];
			*(dst++) = this->p[i]->v[1];
			*(dst++) = this->p[i]->v[2];
		}
	}

	void setState( float *src ){
		for( int i = 0; i < this->n; i++ ){
			this->p[i]->x[0] = *(src++);
			this->p[i]->x[1] = *(src++);
			this->p[i]->x[2] = *(src++);
			this->p[i]->v[0] = *(src++);
			this->p[i]->v[1] = *(src++);
			this->p[i]->v[2] = *(src++);
		}
	}

	void clearForces(){
		for( int i = 0; i < this->n; i++ ){
			this->p[i]->f[0] = 0.0;
			this->p[i]->f[1] = 0.0;
			this->p[i]->f[2] = 0.0;
		}
	}

	void computeForces(){
		fos->applyForce( this );
	}

	void derivative( float *dst ){
		this->clearForces();
		this->computeForces();

		for( int i = 0; i < this->n; i++ ){
			*(dst++) = this->p[i]->v[0];
			*(dst++) = this->p[i]->v[1];
			*(dst++) = this->p[i]->v[2];
			*(dst++) = this->p[i]->f[0]/this->p[i]->m;
			*(dst++) = this->p[i]->f[1]/this->p[i]->m;
			*(dst++) = this->p[i]->f[2]/this->p[i]->m;
		}
	}

};

class Solver{
private:
	float delta;
public:
	void scaleVector( float *dst, float delta ){

	}

	void add( float *dst, float *state, float *target ){

	}

	void step( ParticleSystem *ps ){
		float *dst;
		float *state;
		ps->derivative( dst );
		this->scaleVector( dst, this->delta );
		ps->getState( state );
		this->add( dst, state, state );
		ps->setState( state );
		ps->t += delta;
	}
};

class Simulation{
private:
	GLFWwindow* window;
	GLuint shaderId;
	GLuint VAO;
	ParticleSystem *ps;

public:
	// Drawing routines
	void init(){
		GLuint VBO;

		GLfloat particle_shape[]{
			0.0f, 1.0f, 0.0f, 1.0f,
		    1.0f, 0.0f, 1.0f, 0.0f,
		    0.0f, 0.0f, 0.0f, 0.0f,

		    0.0f, 1.0f, 0.0f, 1.0f,
		    1.0f, 1.0f, 1.0f, 1.0f,
		    1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays( 1, &this->VAO );
		glGenBuffers( 1, &VBO );
		glBindVertexArray( this->VAO );
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(particle_shape), particle_shape, GL_STATIC_DRAW );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)0 );



	}

	void draw( ParticleSystem *ps ){

	}

	int run()
	{
		bool running = true;
		int n = 30;
		ps = new ParticleSystem( n );
		Solver s;

		while( running ){
			s.step(ps);
			draw( ps );
		}
		
		return 0;
	}

};

int main( int c, char **args ){

}
