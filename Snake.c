#include "Snake.h"
#include <stdio.h>

float TILE_SIZE = 30.f;
int GRID_START_X = 30;
int GRID_START_Y = 150;

int grid[GRID_WIDTH][GRID_HEIGHT] = { 0 };
float snake_speed_timer = 0.0f;

int food_exists = 0;
int game_over = 0;
char* text = "";
struct Snake_Profile snake1;

void Level_Init()
{
	CP_System_SetWindowSize(900, 900);
	// Adjust tile size according to level size
	TILE_SIZE = (((float)CP_System_GetWindowHeight() - (float)GRID_START_Y - 5) / (float)GRID_HEIGHT);
	// Centers the level
	GRID_START_X = (int)((float)CP_System_GetWindowWidth() - (GRID_WIDTH * TILE_SIZE)) / 2;
}

void Snake_Init()
{
	// add snake of size 1 at (0,0)
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
	snake1.HeadColor = GREEN;
	snake1.BodyColor = DARK_GREEN;
	Snake_GrowSnake(0, 0, &snake1);
}

void Snake_Update(const float dt)
{
	CP_Settings_Background((CP_Color) { 255, 255, 255, 255 });
	Snake_UpdateSnake(dt);
	if (CP_Input_MouseClicked()) {
		Snake_SpawnFood();
		snake1.to_grow = 1;
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
		float x0 = GRID_START_X + x * TILE_SIZE;
		CP_Graphics_DrawLine(x0, (float)GRID_START_Y, x0, (float)GRID_START_Y + GRID_HEIGHT * TILE_SIZE);
	}
	for (int y = 0; y < GRID_WIDTH+1; y++) {
		float y0 = GRID_START_Y + y * TILE_SIZE;
		CP_Graphics_DrawLine((float)GRID_START_X, y0, (float)GRID_START_X + GRID_WIDTH * TILE_SIZE, y0);
	}
	// render snake
	Snake_DrawSnake();

	// render food
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[y][x] == 2) {
				CP_Settings_Fill(GREY);
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
		if (i == 0)
		{ CP_Settings_Fill(snake1.HeadColor); }
		else { CP_Settings_Fill(snake1.BodyColor); }
		CP_Graphics_DrawRect(snake1.Position[i].x * TILE_SIZE + GRID_START_X, snake1.Position[i].y * TILE_SIZE + GRID_START_Y, TILE_SIZE, TILE_SIZE);		
	}
}

void Snake_UpdateSnake(const float dt)
{
	// SNAKE INPUT
	if (CP_Input_KeyDown(KEY_RIGHT) && snake1.PreviousDirection != Left) {
		snake1.Direction = Right;
	}
	if (CP_Input_KeyDown(KEY_LEFT) && snake1.PreviousDirection != Right) {
		snake1.Direction = Left;
	}
	if (CP_Input_KeyDown(KEY_UP) && snake1.PreviousDirection != Down) {
		snake1.Direction = Up;
	}
	if (CP_Input_KeyDown(KEY_DOWN) && snake1.PreviousDirection != Up) {
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
		snake1.PreviousDirection = snake1.Direction;
		// if snake collide with food
		if (grid[(int)snake1.Position[0].y][(int)snake1.Position[0].x] == 2) {
			snake1.to_grow = 1;
			//Check_For_Food();
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
			Snake_GrowSnake((int)last_position.x, (int)last_position.y, &snake1);
			snake1.to_grow = 0;
		}
		else { // else update grid to let know empty
			grid[(int)last_position.y][(int)last_position.x] = 0;
		}
		// set new position of head in grid
		grid[(int)snake1.Position[0].y ][(int)snake1.Position[0].x] = 1;

		Check_For_Food();
	}

	if (CP_Input_KeyDown(KEY_R))
	{
		Reset_Game();
	}
}

void Snake_GrowSnake(const int x, const int y, struct Snake_Profile *snake)
{

	if (snake->Size < GRID_WIDTH * GRID_HEIGHT) {
		snake->Position[snake->Size++] = (CP_Vector){ (float)x,(float)y };
		grid[y][x] = 1;
	}
	else {
		printf("You won the game!");
	}
}

void Snake_SpawnFood()
{
	int rand_x;
	int rand_y;
	int check = 1;
	while (check) {
		rand_x = CP_Random_RangeInt(0, GRID_WIDTH-1);
		rand_y = CP_Random_RangeInt(0, GRID_HEIGHT-1);
		// if not at a snake position
		if (grid[rand_y][rand_x] != 1) {
			check = 0;
		}
	}
	grid[rand_y][rand_x] = 2;
}

void Reset_Game()
{
	Level_Init();
	Snake_Init();
}

void Check_For_Food()
{
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (grid[i][j] == 2)
			{
				food_exists = 1;
				return;
			}
		}
	}
	food_exists = 0;
	return;
}