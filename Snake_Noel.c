#include "Snake_Noel.h"
#include <stdlib.h>

int TILE_SIZE_NOEL = 30;	// size of each arena tile
float timer = 0;
int level_grid[GRID_WIDTH_NOEL][GRID_HEIGHT_NOEL];
float snakeSpeed_ry = 30.f;

void Level_Init_Noel()
{
	CP_System_SetWindowSize(900, 900);
	TILE_SIZE_NOEL = (CP_System_GetWindowWidth() - 60) / GRID_WIDTH_NOEL;
	for (int i = 0; i < GRID_WIDTH_NOEL; i++)
	{
		for (int j = 0; j < GRID_HEIGHT_NOEL; j++)
		{
			level_grid[i][j] = 0;
		}
	}
	level_grid[rand() % GRID_WIDTH_NOEL][rand() % GRID_HEIGHT_NOEL] = 1;
}

void Snake_Init_Noel()
{
	sneko.snake_head_x = 1;
	sneko.snake_head_y = 0;
	sneko.snake_direction = 0;
	sneko.snake_speed = 10.f / snakeSpeed_ry;
	sneko.is_alive = 1;
	sneko.is_growing = 0;
	for (unsigned int i = 0; i < 127; i++)
	{
		sneko.snake_body_x[i] = -1;
		sneko.snake_body_y[i] = -1;
	}

	sneko.snake_body_x[0] = 0;
	sneko.snake_body_y[0] = 0;
	sneko.snake_body_x[1] = 0;
	sneko.snake_body_y[1] = 1;
}

void Snake_Update_Noel(const float dt)
{
	if (sneko.is_alive)
	{
		Snake_Input_Noel();
		Snake_Movement_Noel(dt);
	}
}

void Snake_Movement_Noel(const float dt)
{
	if ((timer += dt) > sneko.snake_speed)
	{
		timer = 0;
		for (int i = 126; i >= 0; i--)
		{
			if (sneko.snake_body_x[i] == -1)
			{
				continue;
			}
			if (i == 0)
			{
				sneko.snake_body_x[i] = sneko.snake_head_x;
				sneko.snake_body_y[i] = sneko.snake_head_y;
			}
			else
			{
				if (sneko.is_growing && i != 126)
				{
					sneko.snake_body_x[i+1] = sneko.snake_body_x[i];
					sneko.snake_body_y[i+1] = sneko.snake_body_y[i];
					sneko.is_growing = 0;
				}
				sneko.snake_body_x[i] = sneko.snake_body_x[i-1];
				sneko.snake_body_y[i] = sneko.snake_body_y[i-1];
			}
		}

		sneko.snake_direction_previous = sneko.snake_direction;
		switch (sneko.snake_direction)
		{
		case 0:
			sneko.snake_head_x++;
			break;
		case 90:
			sneko.snake_head_y--;
			break;
		case 180:
			sneko.snake_head_x--;
			break;
		case 270:
			sneko.snake_head_y++;
			break;
		}

		Snake_Collision_Check_Level();
		Snake_Death_Check();
	}
}

void Snake_Collision_Check_Level()
{
	switch(level_grid[sneko.snake_head_x][sneko.snake_head_y])
	{
	case 1:
		level_grid[sneko.snake_head_x][sneko.snake_head_y] = 0;
		level_grid[rand() % GRID_WIDTH_NOEL][rand() % GRID_HEIGHT_NOEL] = 1;
		sneko.is_growing = 1;
	}
}

void Snake_Input_Noel()
{
	if (CP_Input_KeyDown(KEY_RIGHT) && sneko.snake_direction_previous != 180) {
		sneko.snake_direction = 0;
	}
	if (CP_Input_KeyDown(KEY_LEFT) && sneko.snake_direction_previous != 0) {
		sneko.snake_direction = 180;
	}
	if (CP_Input_KeyDown(KEY_UP) && sneko.snake_direction_previous != 270) {
		sneko.snake_direction = 90;
	}
	if (CP_Input_KeyDown(KEY_DOWN) && sneko.snake_direction_previous != 90) {
		sneko.snake_direction = 270;
	}
}

void Snake_Death_Check()
{
	if ((sneko.snake_head_x < 0) || (sneko.snake_head_x > GRID_WIDTH_NOEL -1)
		|| (sneko.snake_head_y < 0) || (sneko.snake_head_y > GRID_HEIGHT_NOEL -1))
	{
		sneko.is_alive = 0;
	}
}

void Level_Update_Noel()
{
	if (CP_Input_KeyDown(KEY_R)) {
		Level_Init_Noel();
		Snake_Init_Noel();
	}
}

void Level_Render_Noel()
{
	CP_Settings_Background(CP_Color_Create(255,255,255,255));

	for (int x = 0; x < GRID_WIDTH_NOEL + 1; x++) {
		int x0 = 30 + x * TILE_SIZE_NOEL;
		CP_Graphics_DrawLine((float)x0, 30, (float)x0, 30 + GRID_HEIGHT_NOEL * (float)TILE_SIZE_NOEL);
	}
	for (int y = 0; y < GRID_WIDTH_NOEL + 1; y++) {
		int y0 = 30 + y * TILE_SIZE_NOEL;
		CP_Graphics_DrawLine(30, (float)y0, 30 + GRID_WIDTH_NOEL * (float)TILE_SIZE_NOEL, (float)y0);
	}

	for (int i = 0; i < GRID_HEIGHT_NOEL; i++)
	{
		for (int j = 0; j < GRID_WIDTH_NOEL; j++)
		{
			switch (level_grid[i][j])
			{
			case 1: //food
				CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
				CP_Graphics_DrawCircle((i * (float)TILE_SIZE_NOEL + 30 + ((float)TILE_SIZE_NOEL * 0.5f)), (j * (float)TILE_SIZE_NOEL + 30 + ((float)TILE_SIZE_NOEL * 0.5f)), 30);
			}
		}
	}
}

void Snake_Render_Noel()
{
	CP_Settings_Fill(CP_Color_Create(0, 255, 0, 255));
	CP_Graphics_DrawCircle((sneko.snake_head_x * (float)TILE_SIZE_NOEL + 30 + ((float)TILE_SIZE_NOEL * 0.5f)), (sneko.snake_head_y * (float)TILE_SIZE_NOEL + 30 + ((float)TILE_SIZE_NOEL * 0.5f)), 30);
	for (unsigned int i = 0; i < 127; i++)
	{
		if (sneko.snake_body_x[i] == -1)
		{
			break;
		}
		CP_Settings_Fill(CP_Color_Create(0, 155, 0, 255));
		CP_Graphics_DrawCircle((sneko.snake_body_x[i] * (float)TILE_SIZE_NOEL + 30 + ((float)TILE_SIZE_NOEL * 0.5f)), (sneko.snake_body_y[i] * (float)TILE_SIZE_NOEL + 30 + ((float)TILE_SIZE_NOEL * 0.5f)), 30);
	}
}