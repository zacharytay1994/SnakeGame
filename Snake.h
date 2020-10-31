#pragma once
#include "CProcessing/inc/cprocessing.h"
#include <stdio.h>
#include "Powerup.h"

int GRID_WIDTH;	// size of the arena square
int GRID_HEIGHT;

// PLAYER COLORS
#define RED CP_Color_Create(255,0,0,255)
#define DARK_RED CP_Color_Create(155,0,0,255)
#define GREEN CP_Color_Create(0,255,0,255)
#define DARK_GREEN CP_Color_Create(0,155,0,255)
#define BLUE CP_Color_Create(0,0,255,255)
#define DARK_BLUE CP_Color_Create(0,0,155,255)
#define LIGHT_BLUE CP_Color_Create(155, 155, 255, 155)
#define YELLOW CP_Color_Create(255,255,0,255)
#define DARK_YELLOW CP_Color_Create(155,155,0,255)
#define GREY CP_Color_Create(100,100,100,255)
#define BLACK CP_Color_Create(0, 0, 0, 200)

//DEFAULT COLORS
#define WHITE CP_Color_Create(255,255,255,255)

float TILE_SIZE;	// size of each arena tile

int GRID_START_X;
int GRID_START_Y;

//int SNAKE_SPEED;

extern int grid[127][127]; // 0 empty, 1 snake, 2 food, 3 pwrup

typedef enum Snake_Direction {
	Right = 0,
	Left = 180,
	Up = 90,
	Down = 270
} Snake_Direction;

struct Snake_Profile
{
	short Id;
	int Size;
	float Speed;
	float Speed_Multiplier;
	float Speed_Timer;
	CP_Vector Position[127 * 127];
	CP_Vector PositionFollow[127 * 127];
	Snake_Direction PreviousDirection;
	Snake_Direction Direction;
	char to_grow;
	char is_alive;
	char is_exists;
	CP_Color HeadColor;
	CP_Color BodyColor;
	CP_KEY Button_Up;
	CP_KEY Button_Left;
	CP_KEY Button_Right;
	CP_KEY Button_Down;

	int score;
	
};
struct Snake_Profile Players[4];
void Add_Player(short id);

void Level_Init();
char Level_Load(char* filename);
void Snake_Init();
void Snake_Update(const float dt);
void Snake_Render();
void Snake_Free();
void Snake_Restart();
void Snake_Shake();
void Snake_Shake_Update(const float dt);
int Get_NumberOf_Alive_Players();
int Get_NumberOf_Existing_Players();

void Snake_DrawSnake(struct Snake_Profile *snake);
void Snake_UpdateSnake(const float dt, struct Snake_Profile *snake);
void Snake_GrowSnake(const int x, const int y, struct Snake_Profile *snake);
void Snake_Wrap(struct Snake_Profile *snake);

void Snake_SpawnFood();
void Snake_SpawnPwrup(int PowerUpID);

void Reset_Game();
void Check_For_Food();
char Check_For_Empty();