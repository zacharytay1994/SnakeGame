#include "Snake.h"
#include "Particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float TILE_SIZE = 30.f;
int GRID_START_X = 30;
int GRID_START_Y = 150;
int GRID_WIDTH = 19;
int GRID_HEIGHT = 16;
int Windows_Width = 900;
int Windows_Height = 900;

int grid[127][127] = { 0 };
float snake_speed_timer = 0.0f;

int food_exists = 0;
int food_multiplier = 1;
int game_over = 0;
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

char leaderboards_textwinner[100];
char leaderboards_textwinnerscore[100];
char leaderboards_text2[100];
char leaderboards_text3[100];
char leaderboards_text4[100];
char button_playagain_hover;
char button_mainmenu_hover;

char lighting_enabled = 0;

CP_Image snake_background;

/*!
 @brief Generates a basic level
 @param void
 @return void
*/
void Level_Init()
{
	screen_shake_offset = CP_Vector_Set(0.0f, 0.0f);
	snake_background = CP_Image_Load("Assets/snakebg.png");
	//CP_System_SetWindowSize(900, 900);
	food_multiplier = 1;
	button_playagain_hover = 0;
	button_mainmenu_hover = 0;
	for (int i = 0; i < 4; i++)
	{
		Players_Highscore_List[i] = -1;
	}
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

/*
 @brief Loads a level from text file
 @param void
 @return 1 if load successful, 0 if load failed
 */
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

/*
 @brief Initializes snakes and highscore
 @param void
 @return void
*/
void Snake_Init()
{
	Add_Player(0);
	Add_Player(1);
	Add_Player(2);
	Add_Player(3);

	//Init Highscore
	FILE* highscore_read;
	fopen_s(&highscore_read, "highscore.txt", "r");
	if (highscore_read) {
		fscanf_s(highscore_read, "%d", &highscore);
		fclose(highscore_read);
	}
}

/*
 @brief Generates a player profile
 @param id - Depending on the ID, generate a player with specific colour and button controls
 @return void
*/
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
	for (int i = 0; i < 16129; i++)
	{
		snake_new.Lighting[i].x = -1;
		snake_new.Lighting[i].y = -1;
		snake_new.Lighting_Angle[i] = -1;
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

/*
 @brief Updates game
 @param dt - DeltaTime
 @return 1 if return to main menu
*/
char Snake_Update(const float dt)
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
	//DEBUG CHEAT
	//if (CP_Input_MouseClicked()) {
	//	Snake_SpawnFood();
	//	Players[0].to_grow = 1;
	//}
	if (game_over)
	{
		if (CP_Input_GetMouseX() > 250 && CP_Input_GetMouseX() < 650 && CP_Input_GetMouseY() > 600 && CP_Input_GetMouseY() < 700)
		{
			if (CP_Input_MouseClicked())
			{
				button_playagain_hover = 0;
				Reset_Game();
			}
			else
			{
				button_playagain_hover = 1;
			}
		}
		else if (CP_Input_GetMouseX() > 250 && CP_Input_GetMouseX() < 650 && CP_Input_GetMouseY() > 750 && CP_Input_GetMouseY() < 850)
		{
			if (CP_Input_MouseClicked())
			{
				button_mainmenu_hover = 0;
				return 1;
			}
			else
			{
				button_mainmenu_hover = 1;
			}
		}
		else
		{
			button_playagain_hover = 0;
			button_mainmenu_hover = 0;
		}
	}
	if (!food_exists) {
		Snake_SpawnFood();
		if (Get_NumberOf_Alive_Players() > 1 && rand() % 4 == 0)
		{
			Snake_SpawnPwrup(6);
		}
		food_exists = 1;
	}
	if (CP_Input_KeyDown(KEY_R))
	{
		Reset_Game();
	}
	Snake_Shake_Update(dt);

	CP_Settings_TextSize(TILE_SIZE * 0.85f);
	CP_Settings_Fill(BLACK);
	CP_Font_DrawText(timer, (float)(GRID_WIDTH * 2), (float)(GRID_HEIGHT * 3));
	//Timer Interface
	if (!game_over)
	{
		timeCount += (float)dt;
		sprintf_s(timer, 100, "Time: %.2f", timeCount);
	}
	return 0;
}

/*
 @brief Renders all items in the game
 @param void
 @return void
*/
void Snake_Render()
{
	float window_width = (float)CP_System_GetWindowWidth();
	float window_height = (float)CP_System_GetWindowHeight();
	float screen_center_x = window_width / 2.0f;
	float screen_center_y = window_height / 2.0f;
	// render background
	CP_Image_Draw(snake_background, screen_center_x, screen_center_y, window_width + 100.0f, window_height + 100.0f, 100);
	// render the grid x and y
	for (int x = 0; x < GRID_WIDTH+1; x++) {
		float x0 = (GRID_START_X + x * TILE_SIZE) + screen_shake_offset.x;
		CP_Graphics_DrawLine(x0, (float)GRID_START_Y, x0, (float)GRID_START_Y + GRID_HEIGHT * TILE_SIZE);
	}
	for (int y = 0; y < GRID_HEIGHT+1; y++) {
		float y0 = GRID_START_Y + y * TILE_SIZE;
		CP_Graphics_DrawLine((float)GRID_START_X, y0, (float)GRID_START_X + GRID_WIDTH * TILE_SIZE, y0);
	}
	// render snake lighting
	if (lighting_enabled)
	{
		for (int i = 0; i < 4; i++)
		{
			if (Players[i].is_exists == 1)
			{
				if (Players[i].is_alive)
				{
					Draw_Lighting(&Players[i]);
				}
			}
		}
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
				case 6:
				{
					CP_Settings_Fill(LIGHT_BLUE);
					CP_Graphics_DrawCircle((float)x * TILE_SIZE + (TILE_SIZE / 2) + GRID_START_X, (float)y * TILE_SIZE + (TILE_SIZE / 2) + GRID_START_Y, TILE_SIZE);
					break;
				}
			}
		}
	}

	game_over = 1;
	if (Get_NumberOf_Alive_Players())
	{
		game_over = 0;
	}

	CP_Settings_TextSize(TILE_SIZE * 0.85f);
	CP_Font_DrawText(text, (float)CP_System_GetWindowWidth() / 3, (float)GRID_START_Y -5);
	if (game_over) {
		if (Check_For_Empty())
		{
			sprintf_s(text, 127, "GAME OVER!");
		}
		else
		{
			sprintf_s(text, 127, "YOU WIN!");
		}
		if (Players_Highscore_List[0] == -1)
		{
			GameOver_SetHighScore();
		}
		GameOver_Render();
	}
}

void Snake_Free() {

}

/*
 @brief Creates a screen shake effect
 @param void
 @return void
*/
void Snake_Shake()
{
	screen_shake_value = 1.0f;
}

/*
 @brief Update for screen shake effect
 @param dt - DeltaTime
 @return void
*/
void Snake_Shake_Update(const float dt)
{
	screen_shake_offset.x = CP_Random_RangeFloat(-5.0f * screen_shake_value, 5.0f * screen_shake_value);
	screen_shake_offset.y = CP_Random_RangeFloat(-5.0f * screen_shake_value, 5.0f * screen_shake_value);

	if (screen_shake_value > 0.0f) {
		screen_shake_value = CP_Math_LerpFloat(screen_shake_value, 0.0f, CP_System_GetDt() * 4.0f);
	}
}

/*
 @brief Draws the snake and their data
 @param snake - the snake profile to draw
 @return void
*/
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
	CP_Settings_Fill(BLACK);
	if (snake->Id == Get_LeadingPlayer() && !game_over)
	{
		CP_Settings_Fill(RED);
	}
	CP_Settings_TextSize(TILE_SIZE * 0.85f);
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
	if (highscore_read) {
		fscanf_s(highscore_read, "%d", &updated_highscore);
		fclose(highscore_read);
	}

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

/*
 @brief Updates the snake
 @param snake - the snake profile to update
 @return void
*/
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
		snake->score += 1;
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
		// - exceeed bounds
		if ((int)snake->Position[0].y < 0 || (int)snake->Position[0].y >= GRID_HEIGHT || (int)snake->Position[0].x < 0 || (int)snake->Position[0].x >= GRID_WIDTH) {
			Snake_Wrap(snake);
		}
		// if snake collide with food
		if (grid[(int)snake->Position[0].y][(int)snake->Position[0].x] == 2) {
			snake->to_grow = 1;
			Particle_Burst((CP_Vector) { snake->Position[0].x* TILE_SIZE + GRID_START_X, snake->Position[0].y* TILE_SIZE + GRID_START_Y },
				10, 30.0f, 90.0f, 120.0f, 240.0f, 30.0f, 120.0f);
		}
		
		switch(grid[(int)snake->Position[0].y][(int)snake->Position[0].x])
		{
			case 1:
			{
				// game over conditions - hit itself
				snake->is_alive = 0;
				int killer_snake = Get_SnakeFromPosition((int)snake->Position[0].x, (int)snake->Position[0].y);
				sprintf_s(text, 127, "Player %d killed by Player %d!", snake->Id + 1, killer_snake + 1);
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
			case 6:
			{
				if (Get_NumberOf_Alive_Players() > 1)
				{
					int leading_player = Get_LeadingPlayer();
					for (int j = -1; j <= 1; j++)
					{
						for (int i = -1; i <= 1; i++)
						{
							if (grid[(int)Players[leading_player].Position[0].y + j][(int)Players[leading_player].Position[0].x + i] == 0)
							{
								grid[(int)Players[leading_player].Position[0].y + j][(int)Players[leading_player].Position[0].x + i] = 5;
							}
						}
					}
				}
			}
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
	
	if(lighting_enabled)
		Add_Darkness(snake);
	
}

/*
 @brief Wraps the snake around the map if it exits the border
 @param snake - the snake profile to wrap
 @return void
*/
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

/*
 @brief Grows the snake
 @param x, y  - position of the snake to grow at
		snake - the snake profile to grow
 @return void
*/
void Snake_GrowSnake(const int x, const int y, struct Snake_Profile *snake)
{

	if (snake->Size < GRID_WIDTH * GRID_HEIGHT) {
		snake->score += food_multiplier;
		food_multiplier += 1;
		snake->Position[snake->Size] = (CP_Vector){ (float)x,(float)y };
		snake->PositionFollow[snake->Size++] = (CP_Vector){ (float)x,(float)y };
		grid[y][x] = 1;
	}
	else {
		printf("You won the game!");
	}
}

/*
 @brief Spawns the food for the game
 @param void
 @return void
*/
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
}

/*
 @brief Spawns the powerup for the game
 @param id - Depending on the ID, spawn that specific powerup
 @return void
*/
void Snake_SpawnPwrup(int PowerUpID)
{
	int rand_x;
	int rand_y;
	int check = 1;
	while (check) {
		rand_x = CP_Random_RangeInt(0, GRID_WIDTH - 1);
		rand_y = CP_Random_RangeInt(0, GRID_HEIGHT - 1);
		// if not at a snake position & food position
		if (grid[rand_y][rand_x] == 0) {
			check = 0;
		}
	}
	grid[rand_y][rand_x] = PowerUpID;
}

/*
 @brief Sets up the highscore leaderboard
 @param void
 @return void
*/
void GameOver_SetHighScore()
{
	int leading_player = 0;
	int leading_player_score = 0;
	char is_inHighscoreList = 0;
	for (int h = 0; h < 4; h++)
	{
		leading_player = -1;
		leading_player_score = -1;
		for (int i = 0; i < 4; i++)
		{
			is_inHighscoreList = 0;
			if (Players[i].is_exists)
			{
				if (Players[i].score > leading_player_score)
				{
					for (int j = 0; j < 4; j++)
					{
						if (Players_Highscore_List[j] == i)
						{
							is_inHighscoreList = 1;
							break;
						}
					}
					if (is_inHighscoreList == 0)
					{
						leading_player = i;
						leading_player_score = Players[i].score;
					}
				}
			}
		}
		for (int k = 0; k < 4; k++)
		{
			if(Players_Highscore_List[k] == -1)
			{
				Players_Highscore_List[k] = leading_player;
				break;
			}
		}
	}
	sprintf_s(leaderboards_textwinner, 100, "Player %d wins!", Players_Highscore_List[0] + 1);
	sprintf_s(leaderboards_textwinnerscore, 100, "Score: %d", Players[Players_Highscore_List[0]].score);
	sprintf_s(leaderboards_text2, 100, "Player %d\t\t\t\tScore: %3d", Players_Highscore_List[1] + 1, Players[Players_Highscore_List[1]].score);
	sprintf_s(leaderboards_text3, 100, "Player %d\t\t\t\tScore: %3d", Players_Highscore_List[2] + 1, Players[Players_Highscore_List[2]].score);
	sprintf_s(leaderboards_text4, 100, "Player %d\t\t\t\tScore: %3d", Players_Highscore_List[3] + 1, Players[Players_Highscore_List[3]].score);
}

/*
 @brief Renders the leaderboard
 @param void
 @return void
*/
void GameOver_Render()
{
	switch (Players_Highscore_List[0])
	{
	case 0:
	{
		CP_Settings_Fill(DARK_GREEN_TRANSLUCENT);
		break;
	}
	case 1:
	{
		CP_Settings_Fill(DARK_BLUE_TRANSLUCENT);
		break;
	}
	case 2:
	{
		CP_Settings_Fill(DARK_RED_TRANSLUCENT);
		break;
	}
	case 3:
	{
		CP_Settings_Fill(DARK_YELLOW_TRANSLUCENT);
		break;
	}
	}
	CP_Graphics_DrawRect(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());
	CP_Settings_TextAlignment(2, 2);

	// Shadow
	CP_Settings_Fill(BLACK);
	CP_Settings_TextSize(70);
	CP_Font_DrawText(leaderboards_textwinner, (float)Windows_Width / 2 + 2, 250 + 2);
	CP_Font_DrawText(leaderboards_textwinnerscore, (float)Windows_Width / 2 + 2, 320 + 2);
	CP_Settings_TextSize(40);
	if (Players[1].is_exists)
	{
		CP_Font_DrawText(leaderboards_text2, (float)Windows_Width / 2 + 2, 420 + 2);
	}
	if (Players[2].is_exists)
	{
		CP_Font_DrawText(leaderboards_text3, (float)Windows_Width / 2 + 2, 470 + 2);
	}
	if (Players[3].is_exists)
	{
		CP_Font_DrawText(leaderboards_text4, (float)Windows_Width / 2 + 2, 520 + 2);
	}

	if (button_playagain_hover)
	{
		CP_Settings_Fill(GREY);
	}
	CP_Graphics_DrawRect((float)Windows_Width / 2 - 200, 600, 400, 100);
	CP_Settings_Fill(BLACK);
	if (button_mainmenu_hover)
	{
		CP_Settings_Fill(GREY);
	}
	CP_Graphics_DrawRect((float)Windows_Width / 2 - 200, 750, 400, 100);

	CP_Settings_Fill(WHITE);
	CP_Settings_TextSize(70);
	CP_Font_DrawText(leaderboards_textwinner, (float)Windows_Width / 2, 250);
	CP_Font_DrawText(leaderboards_textwinnerscore, (float)Windows_Width / 2, 320);
	CP_Settings_TextSize(40);
	if (Players[1].is_exists)
	{
		CP_Font_DrawText(leaderboards_text2, (float)Windows_Width / 2, 420);
	}
	if (Players[2].is_exists)
	{
		CP_Font_DrawText(leaderboards_text3, (float)Windows_Width / 2, 470);
	}
	if (Players[3].is_exists)
	{
		CP_Font_DrawText(leaderboards_text4, (float)Windows_Width / 2, 520);
	}
	
	CP_Settings_TextAlignment(1, 1);

	CP_Settings_TextSize(80);
	CP_Font_DrawText("PLAY AGAIN", (float)Windows_Width / 2 - 175, 675);
	CP_Font_DrawText("MAIN MENU", (float)Windows_Width / 2 - 175, 825);
}

/*
 @brief Resets the game
 @param void
 @return void
*/
void Reset_Game()
{
	Level_Init();
	Snake_Init();
	sprintf_s(text, 127, "");
	timeCount = 0.0f;
}

/*
 @brief Checks if food already exists in the map
 @param void
 @return void
*/
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

/*
 @brief Checks if an empty spot exists in the map
 @param void
 @return 1 if an empty spot exists, 0 if there are no empty spots
*/
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

/*
 @brief Gets the number of alive players (players playing and not dead)
 @param void
 @return number of alive players
*/
int Get_NumberOf_Alive_Players()
{
	int counter = 0;
	for (int i = 0; i < 4; i++)
	{
		if (Players[i].is_exists && Players[i].is_alive)
		{
			counter++;
		}
	}
	return counter;
}

/*
 @brief Gets the number of existing players (players playing, may or may not be dead)
 @param void
 @return number of existing players
*/
int Get_NumberOf_Existing_Players()
{
	int counter = 0;
	for (int i = 0; i < 4; i++)
	{
		if (Players[i].is_exists)
		{
			counter++;
		}
	}
	return counter;
}

int Get_LeadingPlayer()
{
	int leading_player = 0;
	for (int i = 0; i < 4; i++)
	{
		if (Players[i].is_exists && Players[i].is_alive)
		{
			if (Players[i].score > Players[leading_player].score)
			{
				leading_player = i;
			}
		}
	}
	return leading_player;
}

// PROTOTYPE 
int Get_SnakeFromPosition(int x, int y)
{
	if (grid[y][x] != 1)
	{
		return -1;
	}

	for (int h = 0; h < 4; h++)
	{
		if (Players[h].is_exists)
		{
			for (int i = 0; i < Players[h].Size; i++)
			{
				if (Players[h].Position[i].x == x && Players[h].Position[i].y)
				{
					return h;
				}
			}
		}
	}
	return -1;
}

void Add_Darkness(struct Snake_Profile* snake)
{
	// Generate raycasting
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			float T1 = 0.0f;	// T1 > 0, if T1 = 0, no interserction
			if (grid[i][j] == 4)
			{
				for (int u = 0; u < 3; u++)
				{
					for (int p = 0; p < 4; p++)
					{
						T1 = 0.0f;
						// snake = (snake->PositionFollow[0].x  * TILE_SIZE + GRID_START_X + (TILE_SIZE/2)) + Ray_X * T1
						// snake = (snake->PositionFollow[0].y  * TILE_SIZE + GRID_START_y + (TILE_SIZE/2)) + Ray_Y * T1
						float X = j * TILE_SIZE + GRID_START_X;
						float Y = i * TILE_SIZE + GRID_START_Y;
						switch (p)
						{
							case 0:
							{
								X = j * TILE_SIZE + GRID_START_X;
								Y = i * TILE_SIZE + GRID_START_Y;
								if (j > 0 && grid[i][j - 1] == 4) // left
								{
									continue;
								}
								else if (j > 0 && i > 0 && grid[i - 1][j - 1] == 4)
								{
									
								}
								else if (i > 0 && grid[i - 1][j] == 4) // up
								{
									continue;
								}
								break;
							}
							case 1:
							{
								X = j * TILE_SIZE + GRID_START_X + TILE_SIZE;
								Y = i * TILE_SIZE + GRID_START_Y;
								if (j < GRID_WIDTH - 1 && grid[i][j + 1] == 4) // right
								{
									continue;
								}
								else if((j < GRID_WIDTH - 1 && i > 0 && grid[i - 1][j + 1] == 4))
								{
									break;
								}
								else if (i > 0 && grid[i - 1][j] == 4)			// up
								{
									continue;
								}
								break;
							}
							case 2:
							{
								X = j * TILE_SIZE + GRID_START_X;
								Y = i * TILE_SIZE + GRID_START_Y + TILE_SIZE;
								if (i < GRID_HEIGHT - 1 && grid[i + 1][j] == 4)			// down
								{
									continue;
								}
								else if (i < GRID_HEIGHT - 1 && j > 0 && grid[i + 1][j - 1] == 4)
								{

								}
								else if (j > 0 && grid[i][j - 1] == 4)					// left
								{
									continue;
								}
								break;
							}
							case 3:
							{
								X = j * TILE_SIZE + GRID_START_X + TILE_SIZE;
								Y = i * TILE_SIZE + GRID_START_Y + TILE_SIZE;
								if (j < GRID_WIDTH - 1 && grid[i][j + 1] == 4)		// right
								{
									continue;
								}
								else if (j < GRID_WIDTH - 1 && i < GRID_HEIGHT - 1 && grid[i + 1][j + 1] == 4)
								{

								}
								else if (i < GRID_HEIGHT - 1 && grid[i + 1][j] == 4)	// down
								{
									continue;
								}
								break;
							}
						}
						switch (u)
						{
							case 1:
							{
								float hold_X = X;
								float hold_Y = Y;
								X = (float)(hold_X * cos((0.1 * 0.01745f)) - hold_Y * sin((0.1 * 0.01745f)));
								Y = (float)(hold_X * sin((0.1 * 0.01745f)) + hold_Y * cos((0.1 * 0.01745f)));
								break;
							}
							case 2:
							{
								float hold_X = X;
								float hold_Y = Y;
								X = (float)(hold_X * cos((-0.1 * 0.01745f)) - hold_Y * sin((-0.1 * 0.01745f)));
								Y = (float)(hold_X * sin((-0.1 * 0.01745f)) + hold_Y * cos((-0.1 * 0.01745f)));
								break;
							}
						}
						float Ray_X = X - (snake->PositionFollow[0].x * TILE_SIZE + GRID_START_X + (TILE_SIZE * 0.5f));
						float Ray_Y = Y - (snake->PositionFollow[0].y * TILE_SIZE + GRID_START_Y + (TILE_SIZE * 0.5f));
						for (int m = 0; m < GRID_HEIGHT; m++)
						{
							for (int n = 0; n < GRID_WIDTH; n++)
							{
								if (grid[m][n] == 4)
								{
									{
										for (int q = 0; q < 4; q++)
										{
											// line = X2 + X2_D * T2
											// line = Y2 + Y2_D * T2
											float X2 = n * TILE_SIZE + GRID_START_X;
											float Y2 = m * TILE_SIZE + GRID_START_Y;
											float X2_D = TILE_SIZE;
											float Y2_D = TILE_SIZE;
											switch (q)
											{
											case 0:
											{
												X2 = n * TILE_SIZE + GRID_START_X;
												Y2 = m * TILE_SIZE + GRID_START_Y;
												X2_D = TILE_SIZE;
												Y2_D = 0;
												if (m > 0 && grid[m - 1][n] == 4) //up
												{
													continue;
												}
												else if (n > 0 && m > 0 && grid[m - 1][n - 1] == 4) //up-left
												{
													// must check
												}
												else if (n > 0 && grid[m][n - 1] == 4) //left
												{
													continue;
												}
												if (n < GRID_WIDTH - 1)
												{
													for (int s = n + 1; s < GRID_WIDTH; s++)
													{
														if (grid[m][s] == 4)
														{
															X2_D += TILE_SIZE;
														}
														else
														{
															break;
														}
													}
												}
												break;
											}
											case 1:
											{
												X2 = n * TILE_SIZE + GRID_START_X;
												Y2 = m * TILE_SIZE + GRID_START_Y;
												X2_D = 0;
												Y2_D = TILE_SIZE;
												if (n > 0 && grid[m][n - 1] == 4) //left
												{
													continue;
												}
												else if (n > 0 && m > 0 && grid[m - 1][n - 1] == 4) //up-left
												{
													// must check
												}
												else if (m > 0 && grid[m - 1][n] == 4) //up
												{
													continue;
												}
												if (m < GRID_HEIGHT - 1)
												{
													for (int s = m + 1; s < GRID_HEIGHT; s++)
													{
														if (grid[s][n] == 4)
														{
															Y2_D += TILE_SIZE;
														}
														else
														{
															break;
														}
													}
												}
												break;
											}
											case 2:
											{
												X2 = n * TILE_SIZE + GRID_START_X + TILE_SIZE;
												Y2 = m * TILE_SIZE + GRID_START_Y;
												X2_D = 0;
												Y2_D = TILE_SIZE;
												if (n < GRID_WIDTH - 1 && grid[m][n + 1] == 4) //right
												{
													continue;
												}
												else if (n < GRID_WIDTH - 1 && m > 0 && grid[m - 1][n + 1] == 4) //up-right
												{
													// must check
												}
												else if (m > 0 && grid[m - 1][n] == 4) //up
												{
													continue;
												}
												if (m < GRID_HEIGHT - 1)
												{
													for (int s = m + 1; s < GRID_HEIGHT; s++)
													{
														if (grid[s][n] == 4)
														{
															Y2_D += TILE_SIZE;
														}
														else
														{
															break;
														}
													}
												}
												break;
											}
											case 3:
											{
												X2 = n * TILE_SIZE + GRID_START_X;
												Y2 = m * TILE_SIZE + GRID_START_Y + TILE_SIZE;
												X2_D = TILE_SIZE;
												Y2_D = 0;
												if (m < GRID_HEIGHT - 1 && grid[m + 1][n] == 4) //down
												{
													continue;
												}
												else if (n > 0 && m < GRID_HEIGHT - 1 && grid[m + 1][n - 1] == 4) //down-left
												{
													// must check
												}
												else if (n > 0 && grid[m][n - 1] == 4) //left
												{
													continue;
												}
												if (n < GRID_WIDTH - 1)
												{
													for (int s = n + 1; s < GRID_WIDTH; s++)
													{
														if (grid[m][s] == 4)
														{
															X2_D += TILE_SIZE;
														}
														else
														{
															break;
														}
													}
												}
												break;
											}
											}
											float T2 = (Ray_X * (Y2 - (snake->PositionFollow[0].y * TILE_SIZE + GRID_START_Y + (TILE_SIZE * 0.5f))) + Ray_Y * ((snake->PositionFollow[0].x * TILE_SIZE + GRID_START_X + (TILE_SIZE * 0.5f)) - X2)) / (X2_D * Ray_Y - Y2_D * Ray_X);
											if (T2 < 0 || T2 > 1)
											{
												continue;
											}
											float T1_temp = (X2 + X2_D * T2 - (snake->PositionFollow[0].x * TILE_SIZE + GRID_START_X + (TILE_SIZE * 0.5f))) / Ray_X;
											if (T1 == 0 && T1_temp > 0)
											{
												T1 = T1_temp;
											}
											else if (T1_temp < T1 && T1_temp > 0)
											{
												T1 = T1_temp;
											}
										}
									}
								}
							}
						}
						if (T1 > 0)
						{
							for (int z = 0; z < 16129; z++)
							{
								if (snake->Lighting[z].x == -1)
								{
									snake->Lighting[z].x = (snake->PositionFollow[0].x * TILE_SIZE + GRID_START_X + (TILE_SIZE * 0.5f)) + Ray_X * T1;
									snake->Lighting[z].y = (snake->PositionFollow[0].y * TILE_SIZE + GRID_START_Y + (TILE_SIZE * 0.5f)) + Ray_Y * T1;
									float diff_y = snake->Lighting[z].y - (snake->PositionFollow[0].y * TILE_SIZE + GRID_START_Y + (TILE_SIZE * 0.5f));
									float diff_x = snake->Lighting[z].x - (snake->PositionFollow[0].x * TILE_SIZE + GRID_START_X + (TILE_SIZE * 0.5f));
									snake->Lighting_Angle[z] = atan2f((float)fabs(diff_y), (float)fabs(diff_x));
									if (diff_x < 0 && diff_y > 0)
									{
										snake->Lighting_Angle[z] = 3.14159f - snake->Lighting_Angle[z];
									}
									else if (diff_x < 0 && diff_y < 0)
									{
										snake->Lighting_Angle[z] += 3.14159f;
									}
									else if (diff_x > 0 && diff_y < 0)
									{
										snake->Lighting_Angle[z] = 6.28319f - snake->Lighting_Angle[z];
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	// Sort rays
	for (int i = 0; i < 16129 && snake->Lighting_Angle[i] != -1; i++)
	{
		for (int j = 0; j < 16128 -i && snake->Lighting_Angle[j + 1] != -1; j++)
		{
			if (snake->Lighting_Angle[j] > snake->Lighting_Angle[j + 1])
			{
				float temp = snake->Lighting_Angle[j];
				snake->Lighting_Angle[j] = snake->Lighting_Angle[j + 1];
				snake->Lighting_Angle[j + 1] = temp;

				temp = snake->Lighting[j].x;
				snake->Lighting[j].x = snake->Lighting[j + 1].x;
				snake->Lighting[j + 1].x = temp;

				temp = snake->Lighting[j].y;
				snake->Lighting[j].y = snake->Lighting[j + 1].y;
				snake->Lighting[j + 1].y = temp;
			}
		}
	}
}

void Draw_Lighting(struct Snake_Profile* snake)
{
	CP_Settings_Fill(BLACK);
	CP_Graphics_BeginShape();
	for(int i = 16128; i >= 0; i--)
	{
		if (snake->Lighting[i].x == -1)
		{
			continue;
		}	
		CP_Graphics_AddVertex(snake->Lighting[i].x, snake->Lighting[i].y);

		//CP_Graphics_DrawLine((snake->PositionFollow[0].x * TILE_SIZE + GRID_START_X + (TILE_SIZE * 0.5f)), (snake->PositionFollow[0].y * TILE_SIZE + GRID_START_Y + (TILE_SIZE * 0.5f)), snake->Lighting[i].x, snake->Lighting[i].y);
		snake->Lighting[i].x = -1;
		snake->Lighting[i].y = -1;
		snake->Lighting_Angle[i] = -1;
	}
	CP_Graphics_AddVertex((float)(GRID_WIDTH * TILE_SIZE + GRID_START_X), snake->PositionFollow[0].y * TILE_SIZE + GRID_START_Y);
	
	CP_Graphics_AddVertex((float)(GRID_WIDTH * TILE_SIZE + GRID_START_X), (float)(GRID_HEIGHT * TILE_SIZE + GRID_START_Y));
	CP_Graphics_AddVertex((float)(GRID_START_X), (float)(GRID_HEIGHT * TILE_SIZE + GRID_START_Y));
	CP_Graphics_AddVertex((float)(GRID_START_X), (float)(GRID_START_Y));
	CP_Graphics_AddVertex((float)(GRID_WIDTH * TILE_SIZE + GRID_START_X), (float)(GRID_START_Y));
	
	CP_Graphics_AddVertex((float)(GRID_WIDTH * TILE_SIZE + GRID_START_X), snake->PositionFollow[0].y * TILE_SIZE + GRID_START_Y);
	
	CP_Graphics_EndShape();
}