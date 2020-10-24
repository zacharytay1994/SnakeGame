#pragma once
#include "CProcessing/inc/cprocessing.h"

#define GRID_WIDTH 16	// size of the arena square
#define GRID_HEIGHT 16
#define TILE_SIZE 30	// size of each arena tile

#define GRID_START_X 30
#define GRID_START_Y 30

//int SNAKE_SPEED;

extern int grid[GRID_WIDTH][GRID_HEIGHT]; // 0 empty, 1 snake, 2 food
//extern CP_Vector snake[GRID_WIDTH * GRID_HEIGHT];
//extern int snake_size;
//extern float snake_speed_multiplier;

typedef enum Snake_Direction {
	Right = 0,
	Left = 180,
	Up = 90,
	Down = 270
} Snake_Direction;
//extern int snake_direction;

struct Snake_Profile
{
	int Speed;
	float Speed_Multiplier;
	int Size;
	CP_Vector Position[GRID_WIDTH * GRID_HEIGHT];
	Snake_Direction Direction;
	char to_grow;
	char is_alive;
};

void Snake_Init();
void Snake_Update(const float dt);
void Snake_Render();
void Snake_Free();

void Snake_DrawSnake();
void Snake_UpdateSnake(const float dt);
//void Snake_AddSnake(const int x, const int y);
void Snake_AddSnake(const int x, const int y, struct Snake_Profile *snake);

void Snake_GrowSnake();
void Snake_SpawnFood();