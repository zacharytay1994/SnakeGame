//---------------------------------------------------------
// file:	main.c
// author:	[NAME]
// email:	[DIGIPEN EMAIL ADDRESS]
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright � 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "Snake.h"
#include "Particle.h"
#include "GameState.h"
#include "SplashScreen.h"

enum GameState game_state;
#include "Menu.h"

int is_menu = 1;

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	CP_System_SetWindowSize(900, 900);
	CP_System_ShowConsole();
	game_state = SPLASH_SCREEN;
	// initialize variables and CProcessing settings for this gamestate
	SplashScreen_Init();
	Level_Init();
	Snake_Init();
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void)
{
	// check input, update simulation, render etc.
	switch (game_state)
	{
		case SPLASH_SCREEN:
		{
			if (SplashScreen_Render(CP_System_GetDt()))
			{
				game_state = SNAKE_GAME;
			}
			break;
		}
		case MAIN_MENU:
		{
			break;
		}
		case SNAKE_GAME:
		{
			if (Snake_Update(CP_System_GetDt()))
			{
				//game_state = MAIN_MENU;
			}
			Particle_Update(CP_System_GetDt());
			Particle_Render();
			Snake_Render();
			break;
		}
		case END:
		{
			CP_Engine_Terminate();
			break;
		}
	if (is_menu) {
		Menu_Update();
		Menu_FlagStart(&is_menu);
	}
	else {
		// check input, update simulation, render etc.
		Snake_Update(CP_System_GetDt());
		Particle_Update(CP_System_GetDt());
		Particle_Render();
		Snake_Render();
	}
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
	Snake_Free();
}

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update,game_exit );
	CP_Engine_Run();
	return 0;
}
