#pragma once
#include "CProcessing/inc/cprocessing.h"

#define GRID_WIDTH_NOEL 16	// size of the arena square
#define GRID_HEIGHT_NOEL 16

struct Snake
{
	int snake_head_x;
	int snake_head_y;
	unsigned short snake_direction;
	unsigned short snake_direction_previous;

	int snake_body_x[127];
	int snake_body_y[127];

	float snake_speed;
	char is_alive;
};

struct Snake sneko;

void Level_Init_Noel();
void Snake_Init_Noel();

void Snake_Update_Noel(const float dt);
void Snake_Movement_Noel(const float dt);
void Snake_Death_Check();
void Snake_Input_Noel();

void Level_Render_Noel();
void Snake_Render_Noel();