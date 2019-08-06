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
	float x[3];
	float v[3];
	float f[3];
	GLfloat life;

	Particle():m(1.0f), life(0.0f){
		x = {0.0f, 0.0f, 0.0f};
		v = {0.0f, 0.0f, 0.0f};
		f = {0.0f, 0.0f, 0.0f};
	}
};

class ParticleSystem;

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
	DragForce( ForceObject *fo ):ForceObject(fo){
		this->kd = 0.01;
	}

	void apply( ParticleSystem *ps ){
		for( int i = 0; i < ps->n; i++ ){
			Particle *p = ps->particles[i];
			p->f[0] = this->kd*p->v[0];
			p->f[1] = this->kd*p->v[1];
			p->f[2] = this->kd*p->v[2];
		}	
	}
};

// Particle system
class ParticleSystem{

public:
	vector<Particle *> particles;
	ForceObject *fos;
	int n;
	float t;

	ParticleSystem( int n ) : n(10){
		fos = (ForceObject) new GravityForce( new DragForce(NULL) );
		this->n = n;
		t = 0.0;
	}

	void init(){
		for( int i = 0; i < this->n; i++ ){
			particles.push_back( new Particle() );
		}
	}

	int particleDims(){
		return ( 6 * this->n );
	}

	void getState( float *dst ){
		for( int i = 0; i < this->n; i++ ){
			*(dst++) = this->particles[i]->x[0];
			*(dst++) = this->particles[i]->x[1];
			*(dst++) = this->particles[i]->x[2];
			*(dst++) = this->particles[i]->v[0];
			*(dst++) = this->particles[i]->v[1];
			*(dst++) = this->particles[i]->v[2];
		}
	}

	void setState( float *src ){
		for( int i = 0; i < this->n; i++ ){
			this->particles[i]->x[0] = *(src++);
			this->particles[i]->x[1] = *(src++);
			this->particles[i]->x[2] = *(src++);
			this->particles[i]->v[0] = *(src++);
			this->particles[i]->v[1] = *(src++);
			this->particles[i]->v[2] = *(src++);
		}
	}

	void clearParticles(){
		for( int i = 0; i < this->n; i++ ){
			this->particles[i]->f[0] = 0.0;
			this->particles[i]->f[1] = 0.0;
			this->particles[i]->f[2] = 0.0;

			this->particles[i]->life -= 0.01; // This should be put somewhere else?
		}
	}

	void reSpawn(){
		GLfloat offset;

		for( int i = 0; i < this->n; i++ ){
			Particle *p = this->particles[i];

			if( p->life < 0.0f ){
				offset = (rand() % 100 - 50.0)/10.0;
				p->x[0] += offset;
				p->x[1] += offset;
				p->x[2] += offset;
				p->v = {0.0, 0.0, 0.0};
		}
	}

	void computeForces(){
		fos->applyForce( this );
	}

	void derivative( float *dst ){
		this->clearParticles();
		this->computeForces();

		for( int i = 0; i < this->n; i++ ){
			*(dst++) = this->particles[i]->v[0];
			*(dst++) = this->particles[i]->v[1];
			*(dst++) = this->particles[i]->v[2];
			*(dst++) = this->particles[i]->f[0]/this->particles[i]->m;
			*(dst++) = this->particles[i]->f[1]/this->particles[i]->m;
			*(dst++) = this->particles[i]->f[2]/this->particles[i]->m;
		}
	}

};

class Solver{
private:
	float delta;
public:
	void scaleVector( ParticleSystem *ps, float *dst, float delta ){
		for( int i = 0; i < ps->particleDims(); i++ ){
			dst[i] *= delta;
		}
	}

	void add( ParticleSystem *ps, float *dst, float *state, float *target ){
		for( int i = 0; i < ps->particleDims(); i++ ){
			target[i] = state[i] + dst[i];
		}
	}

	void step( ParticleSystem *ps ){
		float *dst = (float*) malloc( ps->particleDims()*sizeof(float) );
		float *state = (float*) malloc( ps->particleDims()*sizeof(float) );
		ps->derivative( dst );
		this->scaleVector( ps, dst, this->delta );
		ps->getState( state );
		this->add( ps, dst, state, state );
		ps->setState( state );
		ps->t += delta;
	}
};

class Simulation{
private:
	GLFWwindow* window;
	GLuint shaderId;
	GLuint offsetId;
	GLuint colorId;
	GLuint VAO;
	ParticleSystem *ps;
	int n;

public:

	Simulation( int n ){
		this->n = n;
	}
	// Drawing routines
	void init(){
		GLuint VBO;

		// GLfloat particle_shape[]{
		// 	0.0f, 1.0f, 0.0f, 1.0f,
		//     1.0f, 0.0f, 1.0f, 0.0f,
		//     0.0f, 0.0f, 0.0f, 0.0f,

		//     0.0f, 1.0f, 0.0f, 1.0f,
		//     1.0f, 1.0f, 1.0f, 1.0f,
		//     1.0f, 0.0f, 1.0f, 0.0f
		// };

		GLfloat particle_shape[]{
			0.0f, 1.0f, 0.0f,
		    1.0f, 0.0f, 0.0f,
		    0.0f, 0.0f, 0.0f,

		    0.0f, 1.0f, 0.0f,
		    1.0f, 1.0f, 0.0f,
		    1.0f, 0.0f, 0.0f
		};

		glGenVertexArrays( 1, &this->VAO );
		glGenBuffers( 1, &VBO );
		glBindVertexArray( this->VAO );
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(particle_shape), particle_shape, GL_STATIC_DRAW );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0 );

		// Loading shaders
		this->shaderId = LoadShaders( "ParticlesVertexShader.vsh", "ParticlesFragmentShader.fsh" );
		this->offsetId = glGetUniformLocation( this->shaderId, "offset");
		this->colorId = glGetUniformLocation( this->shaderId, "color");

		// Setting up the particle system
		ps = new ParticleSystem( n );
		ps->init();
	}


	void draw( ParticleSystem *ps ){
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		glUseProgram( this->shaderId );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		vec4 color = vec4(1.0, 1.0, 0.0, 1.0);

		for( Particle *p : ps->particles ){
			glUniform3fv( this->offsetId, p->x[0], p->x[1], p->x[2] );
			glUniform4fv( this->colorId, 4, color );
			glBindVertexArray( this->VAO );
			glDrawArrays( GL_TRIANGLES, 0, 6 );
			glBindVertexArray(0);
		}
	}

	int run()
	{
		bool running = true;
		int n = 30;
		
		Solver s;

		while( running ){
			ps->reSpawn();
			s.step(ps);
			draw( ps );
		}
		
		return 0;
	}

};

int main( int c, char **args ){
	Simulation s(100);
	s.run();
}
