#include "Particle.h"

Particle particles[MAX_PARTICLES] = { 0 };
int particle_size = 0;

void Particle_Add(const CP_Vector position, const CP_Vector force, const float size, const float angular, const ParticleColor color)
{
	// dirty method sue me
	for (int i = 0; i < particle_size; ++i) {
		if (!particles[i]._alive) {
			particles[i]._position = position;
			particles[i]._force = force;
			particles[i]._alive = 1;
			particles[i]._size = size;
			particles[i]._rotation = 0.0f;
			particles[i]._angular_velocity = angular;
			particles[i]._color = color;
			particles[i]._alpha = 255;
			return;
		}
	}
	if (particle_size < MAX_PARTICLES) {
		particles[particle_size++] = (Particle){ position,force,1,size,0.0f,angular,color,255 };
	}
}

void Particle_Burst(const CP_Vector position, const int number, const float lower_size, const float upper_size,
	const float lower_force, const float upper_force, const float lower_rotation, const float upper_rotation)
{
	for (int i = 0; i < number; ++i) {
		float random_x_dir = CP_Random_RangeFloat(-1.0f, 1.0f);
		float random_y_dir = CP_Random_RangeFloat(-1.0f, 1.0f);
		float random_force = CP_Random_RangeFloat(lower_force, upper_force);
		float random_size = CP_Random_RangeFloat(lower_size, upper_size);
		float random_angular_velocity = CP_Random_RangeFloat(lower_rotation, upper_rotation);
		int random_color = CP_Random_RangeInt(0, 3);
		Particle_Add(position, (CP_Vector) { random_x_dir* random_force, random_y_dir* random_force }, random_size, random_angular_velocity, random_color);
	}
}

void Particle_Update(const float dt)
{
	for (int i = 0; i < particle_size; ++i) {
		if (particles[i]._alive) {
			particles[i]._position = CP_Vector_Add(particles[i]._position, CP_Vector_Scale(particles[i]._force, CP_System_GetDt()));
			particles[i]._rotation += particles[i]._angular_velocity * CP_System_GetDt();
			if ((particles[i]._alpha -= 4) < 0) {
				particles[i]._alive = 0;
			}
		}
	}
}

void Particle_Render()
{
	for (int i = 0; i < particle_size; ++i) {
		if (particles[i]._alive) {
			switch (particles[i]._color) {
			case Red:
				CP_Settings_Fill(CP_Color_Create(255, 0, 0, particles[i]._alpha));
				break;
			case Yellow:
				CP_Settings_Fill(CP_Color_Create(255, 255, 0, particles[i]._alpha));
				break;
			case Blue:
				CP_Settings_Fill(CP_Color_Create(0, 0, 255, particles[i]._alpha));
				break;
			case Green:
				CP_Settings_Fill(CP_Color_Create(0, 255, 0, particles[i]._alpha));
				break;
			}
			CP_Graphics_DrawRectAdvanced(particles[i]._position.x, particles[i]._position.y, particles[i]._size, particles[i]._size, particles[i]._rotation, 0.0f);
		}
	}
}
