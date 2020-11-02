
#include "CProcessing/inc/cprocessing.h"
#include <stdio.h>
#include "Menu.h"
#include <stdlib.h>

#define MENU_BUTTON_WIDTH 260
#define MENU_BUTTON_HEIGHT 70
#define MENU_BUTTON_X 450.0f
#define MENU_BUTTON_Y 250.0f

const int _Windows_Width = 900;
const int _Windows_Height = 900;

int Menu_is_menu = 1;

int Menu_High_Score = 0;
int Menu_Instruction = 0;
int Menu_Return = 0;


void Menu_Init(void)
{
}

void Menu_Update(void)
{
	CP_Settings_Background((CP_Color) 
	{
		255, 255, 255, 255
	});

	
	if (!Menu_High_Score && !Menu_Instruction)
	{
		CP_Image Play1 = CP_Image_Load("Photos/Menu_Button_Play-01.png");
		CP_Image_Draw(Play1, MENU_BUTTON_X, MENU_BUTTON_Y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);

		CP_Image Play2 = CP_Image_Load("Photos/Menu_Button_Instruction-01.png");
		CP_Image_Draw(Play2, MENU_BUTTON_X, MENU_BUTTON_Y + 100.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);

		CP_Image Play3 = CP_Image_Load("Photos/Menu_Button_HighScore-01.png");
		CP_Image_Draw(Play3, MENU_BUTTON_X, MENU_BUTTON_Y + 200.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);

		CP_Image Play4 = CP_Image_Load("Photos/Menu_Button_Quit-01.png");
		CP_Image_Draw(Play4, MENU_BUTTON_X, MENU_BUTTON_Y + 300.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);


		float MouseX = CP_Input_GetMouseX();
		float MouseY = CP_Input_GetMouseY();


		if (!(MouseX < 320 || MouseY < 215 || MouseX > 580 || MouseY > 285))
		{
			CP_Image Play5 = CP_Image_Load("Photos/Menu_Button_Play-02.png");
			CP_Image_Draw(Play5, MENU_BUTTON_X, MENU_BUTTON_Y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);
			if (CP_Input_MouseClicked()) {
				Menu_StartGame();
			}
		}
		else if (!(MouseX < 320 || MouseY < 315 || MouseX > 580 || MouseY > 385))
		{
			CP_Image Play6 = CP_Image_Load("Photos/Menu_Button_Instruction-02.png");
			CP_Image_Draw(Play6, MENU_BUTTON_X, MENU_BUTTON_Y + 100.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);
			if (CP_Input_MouseClicked())
			{
				Menu_Instruction = 1;
			}
		}
		else if (!(MouseX < 320 || MouseY < 415 || MouseX > 580 || MouseY > 485))
		{
			CP_Image Play7 = CP_Image_Load("Photos/Menu_Button_HighScore-02.png");
			CP_Image_Draw(Play7, MENU_BUTTON_X, MENU_BUTTON_Y + 200.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);
			if (CP_Input_MouseClicked())
			{
				Menu_High_Score = 1;
			}
		}
		else if (!(MouseX < 320 || MouseY < 515 || MouseX > 580 || MouseY > 585))
		{
			CP_Image Play8 = CP_Image_Load("Photos/Menu_Button_Quit-02.png");
			CP_Image_Draw(Play8, MENU_BUTTON_X, MENU_BUTTON_Y + 300.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);
			if (CP_Input_MouseClicked()) {
				CP_Engine_Terminate();
			}
		}
	}
	else if (Menu_High_Score)
	{
		int Menu_Score;
		FILE* Menu_Score_Read;
		fopen_s(&Menu_Score_Read, "highscore.txt", "r");

		if (Menu_Score_Read)
		{
			fscanf_s(Menu_Score_Read, "%d", &Menu_Score);
			fclose(Menu_Score_Read);
			char Menu_S_Read[50];
			_itoa_s(Menu_Score, Menu_S_Read, 50,10);
			CP_Settings_Fill((CP_Color){ 0, 0, 0, 255 });
			CP_Settings_TextSize(50.0f);
			CP_Settings_TextAlignment(2,2);
			CP_Font_DrawText(Menu_S_Read, 450.0f, 350.0f);
			CP_Image Play9 = CP_Image_Load("Photos/Menu_Button_Return-01.png");
			CP_Image_Draw(Play9, MENU_BUTTON_X, MENU_BUTTON_Y + 300.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);
			
			float MouseX = CP_Input_GetMouseX();
			float MouseY = CP_Input_GetMouseY();

			if (!(MouseX < 320 || MouseY < 515 || MouseX > 580 || MouseY > 585))
			{
				CP_Image Play10 = CP_Image_Load("Photos/Menu_Button_Return-02.png");
				CP_Image_Draw(Play10, MENU_BUTTON_X, MENU_BUTTON_Y + 300.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);
				if (CP_Input_MouseClicked())
				{
					Menu_High_Score =0;
				}
			}
			
		}	
		
	}
	else if (Menu_Instruction) {
		CP_Image instruction = CP_Image_Load("Photos/Menu_Instruction.png");
		CP_Image_Draw(instruction, CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f, 900.0f, 900.0f, 255);
		CP_Image Play11 = CP_Image_Load("Photos/Menu_Button_Return-01.png");
		CP_Image_Draw(Play11, MENU_BUTTON_X, MENU_BUTTON_Y + 600.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);

		float MouseX = CP_Input_GetMouseX();
		float MouseY = CP_Input_GetMouseY();

		if (!(MouseX < 320 || MouseY < 815 || MouseX > 580 || MouseY > 885))
		{
			CP_Image Play12 = CP_Image_Load("Photos/Menu_Button_Return-02.png");
			CP_Image_Draw(Play12, MENU_BUTTON_X, MENU_BUTTON_Y + 600.0f, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, 255);
			if (CP_Input_MouseClicked())
			{
				Menu_Instruction = 0;
			}
		}

	}
}

void Menu_Exit(void)
{
}

void Menu_FlagStart(int* i)
{
	*i = Menu_is_menu;
}

void Menu_StartGame()
{
	Menu_is_menu = 0;
}


