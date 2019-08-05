#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

GLFWwindow* window;

typedef struct{
	float m;
	float *x;
	float *v;
	float *f;
} *Particle;

typedef struct{
	Particle *p;
	int n;
	float t;
} *ParticleSystem;

int particleDims( ParticleSystem p ){
	return ( 6 * p->n );
}

void particlesGetState( ParticleSystem p, float *dst ){
	for( int i = 0; i < p->n; i++ ){
		*(dst++) = p->p[i]->x[0];
		*(dst++) = p->p[i]->x[1];
		*(dst++) = p->p[i]->x[2];
		*(dst++) = p->p[i]->v[0];
		*(dst++) = p->p[i]->v[1];
		*(dst++) = p->p[i]->v[2];
	}
}

void particleSetState( ParticleSystem p, float *src ){
	for( int i = 0; i < p->n; i++ ){
		p->p[i]->x[0] = *(src++);
		p->p[i]->x[1] = *(src++);
		p->p[i]->x[2] = *(src++);
		p->p[i]->v[0] = *(src++);
		p->p[i]->v[1] = *(src++);
		p->p[i]->v[2] = *(src++);
	}
}

void particleDerivative( ParticleSystem p, float *dst ){
	clearForces( p );
	computeForces( p );

	for( int i = 0; i < p->n; i++ ){
		*(dst++) = p->p[i]->v[0];
		*(dst++) = p->p[i]->v[1];
		*(dst++) = p->p[i]->v[2];
		*(dst++) = p->p[i]->f[0]/p->p[i]->m;
		*(dst++) = p->p[i]->f[1]/p->p[i]->m;
		*(dst++) = p->p[i]->f[2]/p->p[i]->m;
	}
}

void eulerStep( ParticleSystem p, float delta ){
	particleDerivative( p, dst );
	scaleVector( dst, delta );
	particleGetState( p, state );
	add( dst, state, state );
	particleSetState( p, state );
	p->t += delta;
}

// Drawing routines
void init(){



}

int main( void )
{
	bool running = true;

	while( running ){

	}
	
	return 0;
}

