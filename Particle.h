#pragma once
#include "CProcessing/inc/cprocessing.h"

#define MAX_PARTICLES 100

typedef enum ParticleColor {
	Red,
	Yellow,
	Blue,
	Green
} ParticleColor;

typedef struct Particle {
	CP_Vector _position;
	CP_Vector _force;
	int _alive;
	float _size;
	float _rotation;
	float _angular_velocity;
	ParticleColor _color;
	int _alpha;
} Particle;

extern Particle particles[MAX_PARTICLES];

void Particle_Add(const CP_Vector position, const CP_Vector force, const float size, const float angular, const ParticleColor color);
void Particle_Burst(const CP_Vector position, const int number, const float lower_size, const float upper_size,
	const float lower_force, const float upper_force, const float lower_rotation, const float upper_rotation);
void Particle_Update(const float dt);
void Particle_Render();