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

int particlesGetState( ParticleSystem p, float *dst ){
	for( int i = 0; i < p->n; i++ ){
		*(dst++) = p->p[i]->x[0];
		*(dst++) = p->p[i]->x[1];
		*(dst++) = p->p[i]->x[2];
		*(dst++) = p->p[i]->v[0];
		*(dst++) = p->p[i]->v[1];
		*(dst++) = p->p[i]->v[2];
	}
}

// Drawing routines


int main( void )
{
	
	return 0;
}

