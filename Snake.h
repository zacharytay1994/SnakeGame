#pragma once
#include "CProcessing/inc/cprocessing.h"

#define GRID_WIDTH 5	// size of the arena square
#define GRID_HEIGHT 10
#define TILE_SIZE 30	// size of each arena tile

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900

#define GRID_START_X grid_position.x
#define GRID_START_Y grid_position.y

#define SNAKE_SPEED 1
#define GRID_SPEED 30

extern int grid[GRID_WIDTH*GRID_HEIGHT]; // 0 empty, 1 snake, 2 food
extern CP_Vector snake[GRID_WIDTH * GRID_HEIGHT];
extern int snake_size;
extern float snake_speed_multiplier;

extern CP_Vector grid_position;
extern CP_Vector grid_direction;

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
void Snake_Restart();

void Snake_DrawSnake();
void Snake_UpdateSnake(const float dt);
void Snake_AddSnake(const int x, const int y);
void Snake_UpdateSnakeOffGrid(const float dt);

void Snake_GrowSnake();
void Snake_SpawnFood();

void Snake_MoveGrid(const float dt);