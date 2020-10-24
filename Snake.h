#pragma once
#include "CProcessing/inc/cprocessing.h"

#define GRID_WIDTH 16	// size of the arena square
#define GRID_HEIGHT 16

#define RED CP_Color_Create(255,0,0,255)
#define DARK_RED CP_Color_Create(155,0,0,255)
#define GREEN CP_Color_Create(0,255,0,255)
#define DARK_GREEN CP_Color_Create(0,155,0,255)
#define BLUE CP_Color_Create(0,0,255,255)
#define DARK_BLUE CP_Color_Create(0,0,155,255)
#define GREY CP_Color_Create(100,100,100,255)

float TILE_SIZE;	// size of each arena tile

int GRID_START_X;
int GRID_START_Y;

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
	int Size;
	float Speed_Multiplier;
	CP_Vector Position[GRID_WIDTH * GRID_HEIGHT];
	Snake_Direction PreviousDirection;
	Snake_Direction Direction;
	char to_grow;
	char is_alive;
	CP_Color HeadColor;
	CP_Color BodyColor;
};

void Level_Init();
void Snake_Init();
void Snake_Update(const float dt);
void Snake_Render();
void Snake_Free();

void Snake_DrawSnake();
void Snake_UpdateSnake(const float dt);
void Snake_GrowSnake(const int x, const int y, struct Snake_Profile *snake);

void Snake_SpawnFood();

void Reset_Game();
void Check_For_Food();