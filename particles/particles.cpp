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
GravityForce::GravityForce( ForceObject *fo ):ForceObject(fo){}

void GravityForce::apply( ParticleSystem *ps ){
	for( int i = 0; i < ps->n; i++ ){
		ps->particles[i]->f[0] = 0.0;
		ps->particles[i]->f[1] = 0.0;
		ps->particles[i]->f[2] = -10.0;
	}	
}


DragForce::DragForce( ForceObject *fo ):ForceObject(fo){
	this->kd = 0.0;
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
	fos = (ForceObject *) new GravityForce( new DragForce(NULL) );
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
	GLfloat offsetx, offsety, offsetz;

	for( int i = 0; i < this->n; i++ ){
		Particle *p = this->particles[i];

		if( p->life < 0.0f ){
			offsetx = (rand() % 100 - 50.0)/100.0;
			offsety = (rand() % 100 - 50.0)/100.0;
			offsetz = (rand() % 100 - 50.0)/100.0;
			p->x[0] = offsetx;
			p->x[1] = offsety;
			p->x[2] = offsetz;
			p->v[0] = 0.0;
			p->v[1] = 0.0;
			p->v[2] = 0.0;
			p->life = 1.0f;

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

// Solver
Solver::Solver(){
	this->delta = 0.01;
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
	ps->t += this->delta;
}

Simulation::Simulation( int n ){
	this->n = n;
	this->init();
	this->setup();
}

int Simulation::init(){
	int width = 1024;
	int height = 768;
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
    this->window = glfwCreateWindow( width, height, "Playground", NULL, NULL);

    if( this->window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(this->window);
    glewExperimental = true; // Needed for core profile

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.0f, 0.3f, 0.4f, 0.0f);

}

// Drawing routines
void Simulation::setup(){
	cout << "Initializing the simulation" << endl;
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

	cout << "Generating data for OpenGL" <<endl;
	glGenVertexArrays( 1, &(this->VAO) );
	glGenBuffers( 1, &this->VBO );	
	glBindBuffer( GL_ARRAY_BUFFER, this->VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(particle_shape), particle_shape, GL_STATIC_DRAW );
	glBindVertexArray( this->VAO );
	
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0 );
	glEnableVertexAttribArray( 0 );

	cout << "Loading the shaders" << endl;
	// Loading shaders
	this->shaderId = LoadShaders( "ParticlesVertexShader.vsh", "ParticlesFragmentShader.fsh" );
	this->offsetId = glGetUniformLocation( this->shaderId, "offset");
	this->colorId = glGetUniformLocation( this->shaderId, "color");

	cout << "Initializing the particle system" << endl;
	// Setting up the particle system
	ps = new ParticleSystem( this->n );
	ps->init();
}


void Simulation::draw( ParticleSystem *ps ){
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glUseProgram( this->shaderId );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	GLfloat color[4] = {1.0f, 1.0f, 0.0f, 1.0f};

	for( Particle *p : ps->particles ){
		cout << p->x[0] << endl;
		glUniform3fv( this->offsetId, 1, p->x );
		glUniform4fv( this->colorId, 1, color );
		glBindVertexArray( this->VAO );
		glDrawArrays( GL_TRIANGLES, 0, 6 );
		glBindVertexArray(0);
	}
}

int Simulation::run(){
	cout << "Running the simulation" << endl;
	bool running = true;
	
	Solver s;

	while( running && glfwWindowShouldClose(this->window) == 0 ){
		ps->reSpawn();
		s.step(ps);
		draw( ps );

		glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

	// Cleanup VBO and shader
    glDeleteBuffers(1, &this->VBO );
    glDeleteProgram(this->shaderId);
    glDeleteVertexArrays(1, &this->VAO );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
	
	return 0;
}

// main

int main( int c, char **args ){

	Simulation s(500);
	s.run();
}
