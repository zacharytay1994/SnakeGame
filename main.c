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
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#define ISNOEL
#ifdef ISNOEL
#include "Snake_Noel.h"
#else
#include "Snake.h"
#endif // ISNOEL




// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	// initialize variables and CProcessing settings for this gamestate
#ifdef ISNOEL
	Level_Init_Noel();
	Snake_Init_Noel();
#else
	Snake_Init();
#endif
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void)
{
	// check input, update simulation, render etc.
#ifdef ISNOEL
	Snake_Update_Noel(CP_System_GetDt());
	Level_Update_Noel();
	Level_Render_Noel();
	Snake_Render_Noel();
#else
	Snake_Update(CP_System_GetDt());
	Snake_Render();
#endif // ISNOEL

	//Snake_Update(CP_System_GetDt());
	//Level_Update_Noel();
	//Level_Render_Noel();
	//Snake_Update_Noel(CP_System_GetDt());
	//Snake_Render();
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
#ifndef ISNOEL
	Snake_Free();

#endif // !ISNOEL

}

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}
