#include "SplashScreen.h"
#include <stdio.h>

void SplashScreen_Init()
{
	DigiPen_Logo = CP_Image_Load("Assets/DigiPen_RED.png");
	timer = 0.0f;
	alpha = 0;
	fadeOut = 0;
}

char SplashScreen_Render(const float dt)
{
	CP_Settings_Background((CP_Color) { 255, 255, 255, 255 });
	CP_Image_Draw(DigiPen_Logo, 440, 400, 800, 300, alpha);
	timer += dt;
	
	if (alpha < 255 && !fadeOut)
	{
		alpha += counter;
		counter++;
	}
	else if(alpha > 0 && timer < 5)
	{
		fadeOut = 1;
		counter = 0;
		timer += dt;
	}
	else if(alpha > 0)
	{
		counter++;
		alpha -= counter;
	}
	else
	{
		return 1;
	}
	
	return 0;
}