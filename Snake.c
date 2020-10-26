#include "Snake.h"
#include <stdio.h>

int grid[GRID_WIDTH * GRID_HEIGHT] = { 0 };
CP_Vector snake[GRID_WIDTH * GRID_HEIGHT] = { 0 };
int snake_size = 0;
int snake_direction = Right;
float snake_speed_multiplier = 4.0f;
float snake_speed_timer = 0.0f;

int food_exists = 0;
int to_grow = 0;

int game_over = 0;
char* text = "";

int off_grid = 0;

CP_Vector grid_position;
CP_Vector grid_direction;

CP_Image bg1;
CP_Image gridbg;
float screen_center_x = (float)WINDOW_WIDTH / 2.0f;
float screen_center_y = (float)WINDOW_HEIGHT / 2.0f;

void Snake_Init()
{
	CP_System_SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// add snake of size 1 at (0,0)
	grid_direction = CP_Vector_Normalize(CP_Vector_Set(0.3f, 0.8f));
	grid_position = CP_Vector_Set(1.0f, 1.0f);
	Snake_AddSnake(0, 0);

	// stuffs init
	bg1 = CP_Image_Load("Assets/bg1.png");
	gridbg = CP_Image_Load("Assets/gridbg.png");
}

void Snake_Update(const float dt)
{
	CP_Settings_Background((CP_Color) { 255, 255, 255, 255 });
	Snake_UpdateSnake(dt);
	if (!food_exists) {
		Snake_SpawnFood();
		food_exists = 1;
	}

	if (game_over) {
		text = "GAME OVER!";
		Snake_Restart();
	}

	if (CP_Input_KeyDown(KEY_O)) {
		off_grid = 1;
	}

	// flair
	Snake_MoveGrid(dt);
}

void Snake_Render()
{
	// render background
	CP_Image_Draw(bg1, screen_center_x, screen_center_y, WINDOW_WIDTH + 100.0f, WINDOW_HEIGHT + 100.0f, 100);
	// render grid bg
	CP_Image_Draw(gridbg, GRID_START_X + (float)(GRID_WIDTH*TILE_SIZE)/2.0f, GRID_START_Y + (float)(GRID_HEIGHT * TILE_SIZE)/2.0f, GRID_WIDTH*TILE_SIZE, GRID_HEIGHT*TILE_SIZE, 100);
	// render the grid x and y
	for (int x = 0; x < GRID_WIDTH+1; x++) {
		float x0 = GRID_START_X + x * TILE_SIZE;
		CP_Graphics_DrawLine(x0, GRID_START_Y, (float)x0, GRID_START_Y + (float)GRID_HEIGHT * (float)TILE_SIZE);
	}
	for (int y = 0; y < GRID_HEIGHT+1; y++) {
		float y0 = GRID_START_Y + y * TILE_SIZE;
		CP_Graphics_DrawLine(GRID_START_X, y0, GRID_START_X + (float)GRID_WIDTH * (float)TILE_SIZE, y0);
	}
	// render snake
	Snake_DrawSnake();

	// render food
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[y * GRID_WIDTH + x] == 2) {
				CP_Graphics_DrawCircle((float)x * TILE_SIZE + (TILE_SIZE/2) + GRID_START_X, (float)y * TILE_SIZE + (TILE_SIZE/2) + GRID_START_Y, TILE_SIZE);
			}
		}
	}
	CP_Font_DrawText(text, 100.0f, 700.0f);
}

void Snake_Free() {

}

void Snake_Restart()
{
	for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		grid[i] = 0;
	}
	snake_size = 0;
	snake_direction = Right;
	snake_speed_multiplier = 4.0f;
	snake_speed_timer = 0.0f;

	food_exists = 0;
	to_grow = 0;

	game_over = 0;
	text = "";

	grid_position = CP_Vector_Set(1.0f, 1.0f);
	grid_direction = CP_Vector_Normalize(CP_Vector_Set(0.3f, 0.8f));

	Snake_AddSnake(0, 0);
}

void Snake_DrawSnake()
{
	// for each cell in snake, draw a square there of tile size
	for (int i = 0; i < snake_size; i++) {
		CP_Graphics_DrawRect(snake[i].x * TILE_SIZE + GRID_START_X, snake[i].y * TILE_SIZE + GRID_START_Y, TILE_SIZE, TILE_SIZE);
	}
}

void Snake_UpdateSnake(const float dt)
{
	// SNAKE INPUT
	if (CP_Input_KeyDown(KEY_RIGHT) && snake_direction != Left) {
		snake_direction = Right;
	}
	if (CP_Input_KeyDown(KEY_LEFT) && snake_direction != Right) {
		snake_direction = Left;
	}
	if (CP_Input_KeyDown(KEY_UP) && snake_direction != Down) {
		snake_direction = Up;
	}
	if (CP_Input_KeyDown(KEY_DOWN) && snake_direction != Up) {
		snake_direction = Down;
	}
	// UPDATE SNAKE POSITION
	// each cell in snake that is not the head will go to the cell in front, head will go in direction of snake_direction
	// check snake speed and move snake
	if (snake_speed_timer < (float)SNAKE_SPEED / snake_speed_multiplier) {
		snake_speed_timer += dt;
	}
	else { // if timer up move snake once
		snake_speed_timer = 0.0f;
		CP_Vector last_position = snake[snake_size - 1];
		// move snek
		for (int i = snake_size - 1; i > 0; i--) {
			snake[i] = snake[i - 1];
		}
		switch (snake_direction) {
		case Right:
			snake[0].x += 1;
			break;
		case Left:
			snake[0].x -= 1;
			break;
		case Up:
			snake[0].y -= 1;
			break;
		case Down:
			snake[0].y += 1;
			break;
		}
		// if snake collide with food
		if (grid[(int)snake[0].y * GRID_WIDTH + (int)snake[0].x] == 2) {
			Snake_GrowSnake();
			food_exists = 0;
		}
		// game over conditions - hit itself
		if (grid[(int)snake[0].y * GRID_WIDTH + (int)snake[0].x] == 1) {
			game_over = 1;
		}
		// - exceeed bounds
		if ((int)snake[0].y < 0 || (int)snake[0].y >= GRID_HEIGHT || (int)snake[0].x < 0 || (int)snake[0].x >= GRID_WIDTH) {
			game_over = 1;
		}
		// set last position of snake in grid to 0
		if (to_grow) { // if to grow, add a new snake cell at last position
			Snake_AddSnake((int)last_position.x, (int)last_position.y);
			to_grow = 0;
		}
		else { // else update grid to let know empty
			grid[(int)last_position.y * GRID_WIDTH + (int)last_position.x] = 0;
		}
		// set new position of head in grid
		grid[(int)snake[0].y * GRID_WIDTH + (int)snake[0].x] = 1;
	}
}

void Snake_AddSnake(const int x, const int y)
{
	if (snake_size < GRID_WIDTH * GRID_HEIGHT) {
		snake[snake_size++] = (CP_Vector){ (float)x,(float)y };
		grid[y * GRID_WIDTH + x] = 1;
	}
	else {
		printf("You won the game!");
	}
}

void Snake_UpdateSnakeOffGrid(const float dt)
{
}

void Snake_GrowSnake()
{
	snake_speed_multiplier += 0.1f;
	to_grow = 1;
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
		if (grid[rand_y * GRID_WIDTH + rand_x] != 1) {
			check = 0;
		}
	}
	grid[rand_y * GRID_WIDTH + rand_x] = 2;
}

void Snake_MoveGrid(const float dt)
{
	grid_position = CP_Vector_Add(grid_position, CP_Vector_Scale(grid_direction, GRID_SPEED * dt));
	if (grid_position.x < 0.0f || grid_position.x > WINDOW_WIDTH - GRID_WIDTH * TILE_SIZE) {
		grid_direction.x *= -1;
	}
	if (grid_position.y < 0.0f || grid_position.y > WINDOW_HEIGHT - GRID_HEIGHT * TILE_SIZE) {
		grid_direction.y *= -1;
	}
}
