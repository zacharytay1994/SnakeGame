#include "Snake.h"
#include <stdio.h>

float TILE_SIZE = 30.f;
int GRID_START_X = 30;
int GRID_START_Y = 150;
int GRID_WIDTH = 16;
int GRID_HEIGHT = 16;

int grid[127][127] = { 0 };
float snake_speed_timer = 0.0f;

int food_exists = 0;
int game_over = 0;
//char* text = "";
char text[127] = "";
struct Snake_Profile Players[4] = { {0} };

char scoreText[100];
char timer[100];
float timeCount = 0.f;


void Level_Init()
{
	CP_System_SetWindowSize(900, 900);
	// Adjust tile size according to level size
	TILE_SIZE = (((float)CP_System_GetWindowHeight() - (float)GRID_START_Y - 5) / (float)GRID_HEIGHT);
	// Centers the level
	GRID_START_X = (int)((float)CP_System_GetWindowWidth() - (GRID_WIDTH * TILE_SIZE)) / 2;
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			grid[i][j] = 0;
		}
	}
}

void Snake_Init()
{
	Add_Player(0);
	Add_Player(1);
	Add_Player(2);
	Add_Player(3);
}

void Add_Player(short id)
{
	struct Snake_Profile snake_new;
	short starting_position_x = 0;
	short starting_position_y = 0;
	switch (id)
	{
	case 0:
		snake_new.Direction = Right;
		snake_new.HeadColor = GREEN;
		snake_new.BodyColor = DARK_GREEN;
		snake_new.Button_Up = KEY_UP;
		snake_new.Button_Left = KEY_LEFT;
		snake_new.Button_Right = KEY_RIGHT;
		snake_new.Button_Down = KEY_DOWN;
		starting_position_x = 1;
		starting_position_y = 1;
		break;
	case 1:
		snake_new.Direction = Down;
		snake_new.HeadColor = BLUE;
		snake_new.BodyColor = DARK_BLUE;
		snake_new.Button_Up = KEY_W;
		snake_new.Button_Left = KEY_A;
		snake_new.Button_Right = KEY_D;
		snake_new.Button_Down = KEY_S;
		starting_position_x = (short)GRID_WIDTH -2;
		starting_position_y = 1;
		break;
	case 2:
		snake_new.Direction = Left;
		snake_new.HeadColor = RED;
		snake_new.BodyColor = DARK_RED;
		snake_new.Button_Up = KEY_I;
		snake_new.Button_Left = KEY_J;
		snake_new.Button_Right = KEY_L;
		snake_new.Button_Down = KEY_K;
		starting_position_x = (short)GRID_WIDTH - 2;
		starting_position_y = (short)GRID_HEIGHT - 2;
		break;
	case 3:
		snake_new.Direction = Up;
		snake_new.HeadColor = YELLOW;
		snake_new.BodyColor = DARK_YELLOW;
		snake_new.Button_Up = KEY_KP_8;
		snake_new.Button_Left = KEY_KP_4;
		snake_new.Button_Right = KEY_KP_6;
		snake_new.Button_Down = KEY_KP_5;
		starting_position_x = 1;
		starting_position_y = (short)GRID_HEIGHT - 2;
		break;
	}
	for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
	{
		snake_new.Position[i].x = starting_position_x;
		snake_new.Position[i].y = starting_position_y;
	}
	snake_new.Id = id;
	snake_new.Size = 0;
	snake_new.Speed_Multiplier = 4.0f;
	snake_new.Speed = 1;
	snake_new.to_grow = 0;
	snake_new.is_alive = 1;
	snake_new.is_exists = 1;
	snake_new.Speed_Timer = 0.0f;
	Snake_GrowSnake(starting_position_x, starting_position_y, &snake_new);
	snake_new.score = 0;
	Players[id] = snake_new;
}

void Snake_Update(const float dt)
{
	CP_Settings_Background((CP_Color) { 255, 255, 255, 255 });
	CP_Font_DrawText("Press R to restart", (float)(GRID_WIDTH * 35), (float)(GRID_HEIGHT * 7));
	for (int i = 0; i < 4; i++)
	{
		if (Players[i].is_alive == 1)
		{
			Snake_UpdateSnake(dt, &Players[i]);
		}
	}
	if (CP_Input_MouseClicked()) {
		Snake_SpawnFood();
		Players[0].to_grow = 1;
	}
	if (!food_exists) {
		Snake_SpawnFood();
		food_exists = 1;
	}
	if (CP_Input_KeyDown(KEY_R))
	{
		Reset_Game();
	}

	//Timer Interface
	if (!game_over)
	{
		timeCount += (float)dt;
		sprintf_s(timer, 100, "Time: %.2f", timeCount);
	}
	CP_Font_DrawText(timer, (float)(GRID_WIDTH * 2), (float)(GRID_HEIGHT * 3));
}

void Snake_Render()
{
	// render background
	//CP_Image_Draw(bg1, screen_center_x, screen_center_y, WINDOW_WIDTH + 100.0f, WINDOW_HEIGHT + 100.0f, 100);
	//// render grid bg
	//CP_Image_Draw(gridbg, GRID_START_X + (float)(GRID_WIDTH*TILE_SIZE)/2.0f, GRID_START_Y + (float)(GRID_HEIGHT * TILE_SIZE)/2.0f, GRID_WIDTH*TILE_SIZE, GRID_HEIGHT*TILE_SIZE, 100);
	// render the grid x and y
	for (int x = 0; x < GRID_WIDTH+1; x++) {
		float x0 = GRID_START_X + x * TILE_SIZE;
		CP_Graphics_DrawLine(x0, (float)GRID_START_Y, x0, (float)GRID_START_Y + GRID_HEIGHT * TILE_SIZE);
	}
	for (int y = 0; y < GRID_HEIGHT+1; y++) {
		float y0 = GRID_START_Y + y * TILE_SIZE;
		CP_Graphics_DrawLine((float)GRID_START_X, y0, (float)GRID_START_X + GRID_WIDTH * TILE_SIZE, y0);
	}
	// render snake
	for (int i = 0; i < 4; i++)
	{
		if (Players[i].is_exists == 1)
		{
			Snake_DrawSnake(&Players[i]);
		}
	}

	// render food
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[y][x] == 2) {
				CP_Settings_Fill(GREY);
				CP_Graphics_DrawCircle((float)x * TILE_SIZE + (TILE_SIZE/2) + GRID_START_X, (float)y * TILE_SIZE + (TILE_SIZE/2) + GRID_START_Y, TILE_SIZE);
			}
		}
	}
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[y][x] == 3) {
				CP_Settings_Fill(YELLOW);
				CP_Graphics_DrawCircle((float)x * TILE_SIZE + (TILE_SIZE/2) + GRID_START_X, (float)y * TILE_SIZE + (TILE_SIZE/2) + GRID_START_Y, TILE_SIZE);
			}
		}
	}

	game_over = 1;
	for (int i = 0; i < 4; i++)
	{
		if(Players[i].is_exists && Players[i].is_alive)
		{
			game_over = 0;
		}
	}

	if (game_over) {
		//text = "GAME OVER!";
		sprintf_s(text, 127, "GAME OVER!");
	}
	//CP_Font_DrawText(text, 100.0f, 700.0f);
	CP_Settings_TextSize(TILE_SIZE*0.85f);
	CP_Font_DrawText(text, (float)CP_System_GetWindowWidth() / 3, (float)GRID_START_Y);
}

void Snake_Free() {

}

void Snake_DrawSnake(struct Snake_Profile *snake)
{
	// for each cell in snake, draw a square there of tile size
	for (int i = 0; i < snake->Size; i++) {
		if (i == 0)
		{ CP_Settings_Fill(snake->HeadColor); }
		else { CP_Settings_Fill(snake->BodyColor); }
		CP_Graphics_DrawRect(snake->Position[i].x * TILE_SIZE + GRID_START_X, snake->Position[i].y * TILE_SIZE + GRID_START_Y, TILE_SIZE, TILE_SIZE);
	}

	//Score interface
	CP_Settings_Fill(RED);
	switch (snake->Id)
	{
	case 0:
		sprintf_s(scoreText, 100, "P1 Score: %d", snake->score);
		CP_Font_DrawText(scoreText, (float)(GRID_WIDTH * 2), (float)(GRID_HEIGHT * 5));
		break;
	case 1:
		sprintf_s(scoreText, 100, "P2 Score: %d", snake->score);
		CP_Font_DrawText(scoreText, (float)(GRID_WIDTH * 2), (float)(GRID_HEIGHT * 7));
		break;
	case 2:
		sprintf_s(scoreText, 100, "P3 Score: %d", snake->score);
		CP_Font_DrawText(scoreText, (float)(GRID_WIDTH * 17), (float)(GRID_HEIGHT * 5));
		break;
	case 3:
		sprintf_s(scoreText, 100, "P4 Score: %d", snake->score);
		CP_Font_DrawText(scoreText, (float)(GRID_WIDTH * 17), (float)(GRID_HEIGHT * 7));
		break;
	}
}

void Snake_UpdateSnake(const float dt, struct Snake_Profile *snake)
{
	// SNAKE INPUT
	if (CP_Input_KeyDown(snake->Button_Right) && snake->PreviousDirection != Left) {
		snake->Direction = Right;
	}
	if (CP_Input_KeyDown(snake->Button_Left) && snake->PreviousDirection != Right) {
		snake->Direction = Left;
	}
	if (CP_Input_KeyDown(snake->Button_Up) && snake->PreviousDirection != Down) {
		snake->Direction = Up;
	}
	if (CP_Input_KeyDown(snake->Button_Down) && snake->PreviousDirection != Up) {
		snake->Direction = Down;
	}
	// UPDATE SNAKE POSITION
	// each cell in snake that is not the head will go to the cell in front, head will go in direction of snake_direction
	// check snake speed and move snake
	if (snake->Speed_Timer < (float)snake->Speed / snake->Speed_Multiplier) {
		snake->Speed_Timer += dt;
	}
	else { // if timer up move snake once
		snake->Speed_Timer = 0.0f;
		CP_Vector last_position = snake->Position[snake->Size - 1];
		// move snek
		for (int i = snake->Size - 1; i > 0; i--) {
			snake->Position[i] = snake->Position[i - 1];
		}
		switch (snake->Direction) {
		case Right:
			snake->Position[0].x += 1;
			break;
		case Left:
			snake->Position[0].x -= 1;
			break;
		case Up:
			snake->Position[0].y -= 1;
			break;
		case Down:
			snake->Position[0].y += 1;
			break;
		}
		snake->PreviousDirection = snake->Direction;
		// if snake collide with food
		if (grid[(int)snake->Position[0].y][(int)snake->Position[0].x] == 2) {
			snake->to_grow = 1;
			//Check_For_Food();
		}
		// game over conditions - hit itself
		if (grid[(int)snake->Position[0].y][(int)snake->Position[0].x] == 1) {
			//game_over = 1;
			snake->is_alive = 0;
			sprintf_s(text, 127, "Player %d died by snake!", snake->Id + 1);
		}
		// - exceeed bounds
		if ((int)snake->Position[0].y < 0 || (int)snake->Position[0].y >= GRID_HEIGHT || (int)snake->Position[0].x < 0 || (int)snake->Position[0].x >= GRID_WIDTH) {
			//game_over = 1;
			snake->is_alive = 0;
			sprintf_s(text, 127, "Player %d died by border!", snake->Id + 1);
		}
		// set last position of snake in grid to 0
		if (snake->to_grow) { // if to grow, add a new snake cell at last position
			Snake_GrowSnake((int)last_position.x, (int)last_position.y, snake);
			snake->to_grow = 0;
		}
		else { // else update grid to let know empty
			grid[(int)last_position.y][(int)last_position.x] = 0;
		}
		// set new position of head in grid
		if(snake->is_alive)
			grid[(int)snake->Position[0].y ][(int)snake->Position[0].x] = 1;

		Check_For_Food();
	}
}

void Snake_GrowSnake(const int x, const int y, struct Snake_Profile *snake)
{

	if (snake->Size < GRID_WIDTH * GRID_HEIGHT) {
		snake->score++;
		//highscore = fopen("highscore.txt", "w");
		//fscanf(highscore, "%c", snake->score);
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

	//Snake_SpawnPwrup();
}

void Snake_SpawnPwrup()
{
	int rand_x;
	int rand_y;
	int check = 1;
	while (check) {
		rand_x = CP_Random_RangeInt(0, GRID_WIDTH - 1);
		rand_y = CP_Random_RangeInt(0, GRID_HEIGHT - 1);
		// if not at a snake position & food position
		if (grid[rand_y][rand_x] != 1) {
			if(grid[rand_y][rand_x] != 2)
			check = 0;
		}
	}
	grid[rand_y][rand_x] = 3;
}

void Reset_Game()
{
	Level_Init();
	Snake_Init();
	sprintf_s(text, 127, "");
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