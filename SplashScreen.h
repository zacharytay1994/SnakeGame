#pragma once
#include "CProcessing/inc/cprocessing.h"

CP_Image DigiPen_Logo;
float timer;
int alpha;
int counter;
char fadeOut;

void SplashScreen_Init();
char SplashScreen_Render(const float dt);