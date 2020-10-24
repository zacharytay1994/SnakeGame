#include "Snake.h"
#include <stdio.h>

int grid[GRID_WIDTH][GRID_HEIGHT] = { 0 };
//CP_Vector snake[GRID_WIDTH * GRID_HEIGHT] = { 0 };
//int snake_size = 0;
//int snake_direction = Right;
//float snake_speed_multiplier = 4.0f;
float snake_speed_timer = 0.0f;

int food_exists = 0;
//int to_grow = 0;
//int SNAKE_SPEED = 1;
int game_over = 0;
char* text = "";
struct Snake_Profile snake1;

void Snake_Init()
{
	CP_System_SetWindowSize(900, 900);
	// add snake of size 1 at (0,0)
	//Snake_AddSnake(0, 0);
	for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
	{
		snake1.Position[i].x = 0;
		snake1.Position[i].y = 0;
	}
	snake1.Size = 0;
	snake1.Direction = Right;
	snake1.Speed_Multiplier = 4.0f;
	snake1.Speed = 1;
	snake1.to_grow = 0;
	snake1.is_alive = 1;
	Snake_AddSnake(0, 0, &snake1);
}

void Snake_Update(const float dt)
{
	CP_Settings_Background((CP_Color) { 255, 255, 255, 255 });
	Snake_UpdateSnake(dt);
	if (CP_Input_MouseClicked()) {
		Snake_SpawnFood();
		Snake_GrowSnake();
	}
	if (!food_exists) {
		Snake_SpawnFood();
		food_exists = 1;
	}
}

void Snake_Render()
{
	// render the grid x and y
	for (int x = 0; x < GRID_WIDTH+1; x++) {
		int x0 = GRID_START_X + x * TILE_SIZE;
		CP_Graphics_DrawLine((float)x0, (float)GRID_START_Y, (float)x0, (float)GRID_START_Y + GRID_HEIGHT * TILE_SIZE);
	}
	for (int y = 0; y < GRID_WIDTH+1; y++) {
		int y0 = GRID_START_X + y * TILE_SIZE;
		CP_Graphics_DrawLine((float)GRID_START_X, (float)y0, (float)GRID_START_X + GRID_WIDTH * TILE_SIZE, (float)y0);
	}
	// render snake
	Snake_DrawSnake();

	// render food
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[y][x] == 2) {
				CP_Graphics_DrawCircle((float)x * TILE_SIZE + (TILE_SIZE/2) + GRID_START_X, (float)y * TILE_SIZE + (TILE_SIZE/2) + GRID_START_Y, TILE_SIZE);
			}
		}
	}

	if (game_over) {
		text = "GAME OVER!";
	}
	CP_Font_DrawText(text, 100.0f, 700.0f);
}

void Snake_Free() {

}

void Snake_DrawSnake()
{
	// for each cell in snake, draw a square there of tile size
	for (int i = 0; i < snake1.Size; i++) {
		CP_Graphics_DrawRect(snake1.Position[i].x * TILE_SIZE + GRID_START_X, snake1.Position[i].y * TILE_SIZE + GRID_START_Y, TILE_SIZE, TILE_SIZE);
	}
}

void Snake_UpdateSnake(const float dt)
{
	// SNAKE INPUT
	if (CP_Input_KeyDown(KEY_RIGHT) && snake1.Direction != Left) {
		snake1.Direction = Right;
	}
	if (CP_Input_KeyDown(KEY_LEFT) && snake1.Direction != Right) {
		snake1.Direction = Left;
	}
	if (CP_Input_KeyDown(KEY_UP) && snake1.Direction != Down) {
		snake1.Direction = Up;
	}
	if (CP_Input_KeyDown(KEY_DOWN) && snake1.Direction != Up) {
		snake1.Direction = Down;
	}
	// UPDATE SNAKE POSITION
	// each cell in snake that is not the head will go to the cell in front, head will go in direction of snake_direction
	// check snake speed and move snake
	if (snake_speed_timer < (float)snake1.Speed / snake1.Speed_Multiplier) {
		snake_speed_timer += dt;
	}
	else { // if timer up move snake once
		snake_speed_timer = 0.0f;
		CP_Vector last_position = snake1.Position[snake1.Size - 1];
		// move snek
		for (int i = snake1.Size - 1; i > 0; i--) {
			snake1.Position[i] = snake1.Position[i - 1];
		}
		switch (snake1.Direction) {
		case Right:
			snake1.Position[0].x += 1;
			break;
		case Left:
			snake1.Position[0].x -= 1;
			break;
		case Up:
			snake1.Position[0].y -= 1;
			break;
		case Down:
			snake1.Position[0].y += 1;
			break;
		}
		// if snake collide with food
		if (grid[(int)snake1.Position[0].y][(int)snake1.Position[0].x] == 2) {
			Snake_GrowSnake();
			food_exists = 0;
		}
		// game over conditions - hit itself
		if (grid[(int)snake1.Position[0].y][(int)snake1.Position[0].x] == 1) {
			game_over = 1;
		}
		// - exceeed bounds
		if ((int)snake1.Position[0].y < 0 || (int)snake1.Position[0].y >= GRID_HEIGHT || (int)snake1.Position[0].x < 0 || (int)snake1.Position[0].x >= GRID_WIDTH) {
			game_over = 1;
		}
		// set last position of snake in grid to 0
		if (snake1.to_grow) { // if to grow, add a new snake cell at last position
			Snake_AddSnake((int)last_position.x, (int)last_position.y, &snake1);
			snake1.to_grow = 0;
		}
		else { // else update grid to let know empty
			grid[(int)last_position.y][(int)last_position.x] = 0;
		}
		// set new position of head in grid
		grid[(int)snake1.Position[0].y ][(int)snake1.Position[0].x] = 1;
	}
}

void Snake_AddSnake(const int x, const int y, struct Snake_Profile *snake)
{

	if (snake->Size < GRID_WIDTH * GRID_HEIGHT) {
		snake->Position[snake->Size++] = (CP_Vector){ (float)x,(float)y };
		grid[y][x] = 1;
	}
	else {
		printf("You won the game!");
	}
}

void Snake_GrowSnake()
{
	snake1.to_grow = 1;
}

void Snake_SpawnFood()
{
	int rand_x;
	int rand_y;
	int check = 1;
	while (check) {
		rand_x = CP_Random_RangeInt(0, GRID_WIDTH);
		rand_y = CP_Random_RangeInt(0, GRID_HEIGHT);
		// if not at a snake position
		if (grid[rand_y][rand_x] != 1) {
			check = 0;
		}
	}
	grid[rand_y][rand_x] = 2;
}
