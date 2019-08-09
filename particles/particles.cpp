#include "particles.h"


ForceObject::ForceObject( ForceObject *fo ){
	this->nextObject = fo;
}

ForceObject *ForceObject::next(){
	return this->nextObject;
}
	
void ForceObject::applyForce( ParticleSystem *ps ){
	this->apply( ps );

	if( this->nextObject == NULL ) this->next()->applyForce( ps );
}

// Concrete force objects
void GravityForce::apply( ParticleSystem *ps ){
	for( int i = 0; i < ps->n; i++ ){
		ps[i]->p->f[0] = 0.0;
		ps[i]->p->f[1] = 0.0;
		ps[i]->p->f[2] = -10.0;
	}	
}


DragForce::DragForce( ForceObject *fo ):ForceObject(fo){
	this->kd = 0.01;
}

void DragForce::apply( ParticleSystem *ps ){
	for( int i = 0; i < ps->n; i++ ){
		Particle *p = ps->particles[i];
		p->f[0] = this->kd*p->v[0];
		p->f[1] = this->kd*p->v[1];
		p->f[2] = this->kd*p->v[2];
	}	
}

// Particle system
ParticleSystem::ParticleSystem( int n ) : n(10){
	fos = (ForceObject) new GravityForce( new DragForce(NULL) );
	this->n = n;
	t = 0.0;
}

void ParticleSystem::init(){
	for( int i = 0; i < this->n; i++ ){
		particles.push_back( new Particle() );
	}
}

int ParticleSystem::particleDims(){
	return ( 6 * this->n );
}

void ParticleSystem::getState( float *dst ){
	for( int i = 0; i < this->n; i++ ){
		*(dst++) = this->particles[i]->x[0];
		*(dst++) = this->particles[i]->x[1];
		*(dst++) = this->particles[i]->x[2];
		*(dst++) = this->particles[i]->v[0];
		*(dst++) = this->particles[i]->v[1];
		*(dst++) = this->particles[i]->v[2];
	}
}

void ParticleSystem::setState( float *src ){
	for( int i = 0; i < this->n; i++ ){
		this->particles[i]->x[0] = *(src++);
		this->particles[i]->x[1] = *(src++);
		this->particles[i]->x[2] = *(src++);
		this->particles[i]->v[0] = *(src++);
		this->particles[i]->v[1] = *(src++);
		this->particles[i]->v[2] = *(src++);
	}
}

void ParticleSystem::clearParticles(){
	for( int i = 0; i < this->n; i++ ){
		this->particles[i]->f[0] = 0.0;
		this->particles[i]->f[1] = 0.0;
		this->particles[i]->f[2] = 0.0;

		this->particles[i]->life -= 0.01; // This should be put somewhere else?
	}
}

void ParticleSystem::reSpawn(){
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
}

void ParticleSystem::computeForces(){
	fos->applyForce( this );
}

void ParticleSystem::derivative( float *dst ){
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


void Solver::scaleVector( ParticleSystem *ps, float *dst, float delta ){
	for( int i = 0; i < ps->particleDims(); i++ ){
		dst[i] *= delta;
	}
}

void Solver::add( ParticleSystem *ps, float *dst, float *state, float *target ){
	for( int i = 0; i < ps->particleDims(); i++ ){
		target[i] = state[i] + dst[i];
	}
}

void Solver::step( ParticleSystem *ps ){
	float *dst = (float*) malloc( ps->particleDims()*sizeof(float) );
	float *state = (float*) malloc( ps->particleDims()*sizeof(float) );
	ps->derivative( dst );
	this->scaleVector( ps, dst, this->delta );
	ps->getState( state );
	this->add( ps, dst, state, state );
	ps->setState( state );
	ps->t += delta;
}

Simulation::Simulation( int n ){
	this->n = n;
}
// Drawing routines
void Simulation::init(){
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


void Simulation::draw( ParticleSystem *ps ){
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

int Simulation::run(){
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

// main

int main( int c, char **args ){
	Simulation s(100);
	s.run();
}
