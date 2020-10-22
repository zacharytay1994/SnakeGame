#pragma once
#include "CProcessing/inc/cprocessing.h"
#include <stdio.h>

#define GRID_WIDTH 16	// size of the arena square
#define GRID_HEIGHT 16
#define TILE_SIZE 30	// size of each arena tile

#define GRID_START_X 30
#define GRID_START_Y 30

#define SNAKE_SPEED 1

extern int grid[GRID_WIDTH*GRID_HEIGHT]; // 0 empty, 1 snake, 2 food
extern CP_Vector snake[GRID_WIDTH * GRID_HEIGHT];
extern int snake_size;
extern float snake_speed_multiplier;

typedef enum Snake_Direction {
	Right,
	Left,
	Up,
	Down
} Snake_Direction;
extern int snake_direction;

void Snake_Init();
void Snake_Update(const float dt);
void Snake_Render();
void Snake_Free();

void Snake_DrawSnake();
void Snake_UpdateSnake(const float dt);
void Snake_AddSnake(const int x, const int y);

void Snake_GrowSnake();
void Snake_SpawnFood();