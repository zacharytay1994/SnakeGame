#include "Snake.h"
#include "Particle.h"
#include <stdio.h>
#include <stdlib.h>

float TILE_SIZE = 30.f;
int GRID_START_X = 30;
int GRID_START_Y = 150;
int GRID_WIDTH = 19;
int GRID_HEIGHT = 16;

int grid[127][127] = { 0 };
float snake_speed_timer = 0.0f;

int food_exists = 0;
int game_over = 0;
//char* text = "";
char text[127] = "";
struct Snake_Profile Players[4] = { {0} };

CP_Vector screen_shake_offset;
float screen_shake_value;
char scoreText[100];

int highscore;
int updated_highscore;
char highscore_text[100];
char timer[100];
float timeCount = 0.f;

CP_Image snake_background;

void Level_Init()
{
	screen_shake_offset = CP_Vector_Set(0.0f, 0.0f);
	snake_background = CP_Image_Load("Assets/snakebg.png");
	CP_System_SetWindowSize(900, 900);
	if (!Level_Load("TestLevel.txt"))
	{
		for (int i = 0; i < GRID_HEIGHT; i++)
		{
			for (int j = 0; j < GRID_WIDTH; j++)
			{
				grid[i][j] = 0;
			}
		}
	}
	// Adjust tile size according to level size
	if (GRID_WIDTH - 3 > GRID_HEIGHT)
	{
		TILE_SIZE = (((float)CP_System_GetWindowWidth() - (float)GRID_START_X - 5) / (float)GRID_WIDTH);
		GRID_START_Y = CP_System_GetWindowHeight() - 15 - (int)TILE_SIZE * GRID_HEIGHT;
	}
	else
	{
		TILE_SIZE = (((float)CP_System_GetWindowHeight() - (float)GRID_START_Y - 5) / (float)GRID_HEIGHT);
	}
	
	// Centers the level
	GRID_START_X = (int)((float)CP_System_GetWindowWidth() - (GRID_WIDTH * TILE_SIZE)) / 2;
}

char Level_Load(char* filename)
{
	FILE* customlevel;
	fopen_s(&customlevel, filename, "r");
	char tmp = 0;
	if (customlevel)
	{
		for (int j = 0; j < 127; j++)
		{
			for (int i = 0; i < 127; i++)
			{
				tmp = (char)fgetc(customlevel);
				if (tmp != '\n' && tmp != EOF)
				{
					grid[j][i] = (int)tmp - '0';
				}
				else
				{
					if (i <= 2)
					{
						return 0;
					}
					GRID_WIDTH = i;
					break;
				}
			}
			if (tmp == EOF)
			{
				if (j <= 1)
				{
					return 0;
				}
				GRID_HEIGHT = j + 1;
				break;
			}
		}
		fclose(customlevel);
		if (GRID_WIDTH <= 4 || GRID_HEIGHT <= 4)
		{
			return 0;
		}
		return 1;
	}
	return 0;
}

void Snake_Init()
{
	Add_Player(0);
	Add_Player(1);
	Add_Player(2);
	Add_Player(3);

	//Init Highscore
	FILE* highscore_read;
	fopen_s(&highscore_read, "highscore.txt", "r");
	fscanf_s(highscore_read, "%d", &highscore);
	fclose(highscore_read);
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
	snake_new.Speed_Multiplier = 0.25f;
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
	Snake_Shake_Update(dt);

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
	float window_width = (float)CP_System_GetWindowWidth();
	float window_height = (float)CP_System_GetWindowHeight();
	float screen_center_x = window_width / 2.0f;
	float screen_center_y = window_height / 2.0f;
	// render background
	CP_Image_Draw(snake_background, screen_center_x, screen_center_y, window_width + 100.0f, window_height + 100.0f, 100);
	//// render grid bg
	//CP_Image_Draw(gridbg, GRID_START_X + (float)(GRID_WIDTH*TILE_SIZE)/2.0f, GRID_START_Y + (float)(GRID_HEIGHT * TILE_SIZE)/2.0f, GRID_WIDTH*TILE_SIZE, GRID_HEIGHT*TILE_SIZE, 100);
	// render the grid x and y
	for (int x = 0; x < GRID_WIDTH+1; x++) {
		float x0 = (GRID_START_X + x * TILE_SIZE) + screen_shake_offset.x;
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
			switch (grid[y][x])
			{
				case 2:
				{
					CP_Settings_Fill(GREY);
					CP_Graphics_DrawCircle(((float)x * TILE_SIZE + (TILE_SIZE / 2) + GRID_START_X) + screen_shake_offset.x, ((float)y * TILE_SIZE + (TILE_SIZE / 2) + GRID_START_Y) + screen_shake_offset.y, TILE_SIZE);
					break;
				}
				case 3:
				{
					CP_Settings_Fill(YELLOW);
					CP_Graphics_DrawCircle((float)x * TILE_SIZE + (TILE_SIZE / 2) + GRID_START_X, (float)y * TILE_SIZE + (TILE_SIZE / 2) + GRID_START_Y, TILE_SIZE);
					break;
				}
				case 4:
				{
					CP_Settings_Fill(BLACK);
					CP_Graphics_DrawRect((float)x * TILE_SIZE + GRID_START_X, (float)y * TILE_SIZE + GRID_START_Y, TILE_SIZE, TILE_SIZE);
					break;
				}
				case 5:
				{
					CP_Settings_Fill(LIGHT_BLUE);
					CP_Graphics_DrawRect((float)x * TILE_SIZE + GRID_START_X, (float)y * TILE_SIZE + GRID_START_Y, TILE_SIZE, TILE_SIZE);
					break;
				}
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
		if (Check_For_Empty())
		{
			sprintf_s(text, 127, "GAME OVER!");
		}
		else
		{
			sprintf_s(text, 127, "YOU WIN!");
		}
	}
	//CP_Font_DrawText(text, 100.0f, 700.0f);
	CP_Settings_TextSize(TILE_SIZE*0.85f);
	CP_Font_DrawText(text, (float)CP_System_GetWindowWidth() / 3, (float)GRID_START_Y);
}

void Snake_Free() {

}

void Snake_Shake()
{
	screen_shake_value = 1.0f;
}

void Snake_Shake_Update(const float dt)
{
	screen_shake_offset.x = CP_Random_RangeFloat(-5.0f * screen_shake_value, 5.0f * screen_shake_value);
	screen_shake_offset.y = CP_Random_RangeFloat(-5.0f * screen_shake_value, 5.0f * screen_shake_value);
	// dampen
	/*if (screen_shake_offset.x > 1.0f) {
		screen_shake_offset.x = CP_Math_LerpFloat(screen_shake_offset.x, 0.0f, CP_System_GetDt());
	}
	else {
		screen_shake_offset.x = 0.0f;
	}
	if (screen_shake_offset.y > 1.0f) {
		screen_shake_offset.y = CP_Math_LerpFloat(screen_shake_offset.y, 0.0f, CP_System_GetDt());
	}
	else {
		screen_shake_offset.y = 0.0f;
	}*/
	if (screen_shake_value > 0.0f) {
		screen_shake_value = CP_Math_LerpFloat(screen_shake_value, 0.0f, CP_System_GetDt() * 4.0f);
	}
}

void Snake_DrawSnake(struct Snake_Profile *snake)
{
	// for each cell in snake, draw a square there of tile size
	for (int i = 0; i < snake->Size; i++) {
		if (i == 0)
		{
			CP_Settings_Fill(snake->HeadColor);
		}
		else { CP_Settings_Fill(snake->BodyColor); }
		float ratio_moved = snake->Speed_Timer / (float)(snake->Speed * snake->Speed_Multiplier);
		if (snake->is_alive)
		{
			snake->PositionFollow[i].x = CP_Math_LerpFloat(snake->PositionFollow[i].x, snake->Position[i].x, ratio_moved);
			snake->PositionFollow[i].y = CP_Math_LerpFloat(snake->PositionFollow[i].y, snake->Position[i].y, ratio_moved);
		}
		else
		{
			snake->PositionFollow[i].x = snake->Position[i].x;
			snake->PositionFollow[i].y = snake->Position[i].y;
		}
		CP_Graphics_DrawRect((snake->PositionFollow[i].x * TILE_SIZE + GRID_START_X) + screen_shake_offset.x, (snake->PositionFollow[i].y * TILE_SIZE + GRID_START_Y) + screen_shake_offset.y, TILE_SIZE, TILE_SIZE);
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

	//Init Highscore
	FILE* highscore_read;
	fopen_s(&highscore_read, "highscore.txt", "r");
	fscanf_s(highscore_read, "%d", &updated_highscore);
	fclose(highscore_read);

	//Update Highscore data 
	if (game_over)
	{
		if (snake->score > highscore)
		{
			//Saving new Highscore
			FILE* highscore_write;
			fopen_s(&highscore_write, "highscore.txt", "w");
			fprintf_s(highscore_write, "%d", snake->score);
			fclose(highscore_write);

			//Highscore Notif
			CP_Settings_Fill(BLUE);
			char new_scoreNotif[100];
			sprintf_s(new_scoreNotif, 100, "NEW HIGHSCORE!");
			CP_Font_DrawText(new_scoreNotif, (float)(GRID_WIDTH * 37), (float)(GRID_HEIGHT * 3));
			//Update Highscore display
			CP_Settings_Fill(BLACK);
			sprintf_s(highscore_text, 100, "Highscore: %d", snake->score);
			CP_Font_DrawText(highscore_text, (float)(GRID_WIDTH * 37), (float)(GRID_HEIGHT * 5));
		}
	}
	else
	{
		//Highscore Display
		CP_Settings_Fill(BLACK);
		sprintf_s(highscore_text, 100, "Highscore: %d", updated_highscore);
		CP_Font_DrawText(highscore_text, (float)(GRID_WIDTH * 37), (float)(GRID_HEIGHT * 5));
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
	if (snake->Speed_Timer < snake->Speed * snake->Speed_Multiplier) {
		snake->Speed_Timer += dt;
	}
	else { // if timer up move snake once
		snake->Speed_Timer = 0.0f;
		snake->Speed = 1.0f;
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
			Particle_Burst((CP_Vector) { snake->Position[0].x* TILE_SIZE + GRID_START_X, snake->Position[0].y* TILE_SIZE + GRID_START_Y },
				10, 30.0f, 90.0f, 120.0f, 240.0f, 30.0f, 120.0f);
			//Particle_Add((CP_Vector) { snake->Position[0].x* TILE_SIZE + GRID_START_X, snake->Position[0].y* TILE_SIZE + GRID_START_Y }, CP_Vector_Set(1.0f,0.0f), 30.0f);
			//Check_For_Food();
		}
		
		switch(grid[(int)snake->Position[0].y][(int)snake->Position[0].x])
		{
			case 1:
			{
				// game over conditions - hit itself
				snake->is_alive = 0;
				sprintf_s(text, 127, "Player %d died by snake!", snake->Id + 1);
				break;
			}
			case 4:
			{
				// game over conditions - hit wall
				snake->is_alive = 0;
				sprintf_s(text, 127, "Player %d died by wall!", snake->Id + 1);
				break;
			}
			case 5:
			{
				// debuff - slow down - hit water
				snake->Speed = 2.0f;
				break;
			}
		}
		// - exceeed bounds
		if ((int)snake->Position[0].y < 0 || (int)snake->Position[0].y >= GRID_HEIGHT || (int)snake->Position[0].x < 0 || (int)snake->Position[0].x >= GRID_WIDTH) {
			//game_over = 1;
			//snake->is_alive = 0;
			//sprintf_s(text, 127, "Player %d died by border!", snake->Id + 1);
			Snake_Wrap(snake);
		}
		// set last position of snake in grid to 0
		if (snake->to_grow) { // if to grow, add a new snake cell at last position
			Snake_GrowSnake((int)last_position.x, (int)last_position.y, snake);
			Snake_Shake();
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

void Snake_Wrap(struct Snake_Profile* snake)
{
	if ((int)snake->Position[0].y < 0)
	{
		snake->Position[0].y = (float)GRID_WIDTH - 1;
	}
	if ((int)snake->Position[0].y >= (float)GRID_HEIGHT)
	{
		snake->Position[0].y = 0;
	}
	if ((int)snake->Position[0].x < 0)
	{
		snake->Position[0].x = (float)GRID_WIDTH - 1;
	}
	if ((int)snake->Position[0].x >= (float)GRID_WIDTH)
	{
		snake->Position[0].x = 0;
	}
}

void Snake_GrowSnake(const int x, const int y, struct Snake_Profile *snake)
{

	if (snake->Size < GRID_WIDTH * GRID_HEIGHT) {
		snake->score++;
		//highscore = fopen("highscore.txt", "w");
		//fscanf(highscore, "%c", snake->score);
		snake->Position[snake->Size] = (CP_Vector){ (float)x,(float)y };
		snake->PositionFollow[snake->Size++] = (CP_Vector){ (float)x,(float)y };
		grid[y][x] = 1;
	}
	else {
		printf("You won the game!");
	}
}

void Snake_SpawnFood()
{
	if (!Check_For_Empty())
	{
		game_over = 0;
		return;
	}
	int rand_x;
	int rand_y;
	int check = 1;
	while (check) {
		rand_x = CP_Random_RangeInt(0, GRID_WIDTH-1);
		rand_y = CP_Random_RangeInt(0, GRID_HEIGHT-1);
		// if not at a snake position
		if (grid[rand_y][rand_x] == 0) {
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
	timeCount = 0.0f;
}

void Check_For_Food()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
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

char Check_For_Empty()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (grid[j][i] == 0)
			{
				return 1;
			}
		}
	}
	return 0;
}