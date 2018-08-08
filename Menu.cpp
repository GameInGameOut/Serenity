#include "Menu.h"

static int yc1 = 60, yc2 = 60, yc3 = 60; //Setting the starting y coordinate of column 1 and 2
static int iCounter = 0; //The counter of ints, used for selecting
static int selectedInt = -1; //could be zero but -1 cuz why not

void Menu::AddInt(const char* Text, int* Var, int Min, int Max, int Key, int Column)
{
	iCounter++;

	static Color off = Color(255, 255, 255, 255);
	static Color on = Color(255, 255, 0, 255);

	int x = Column * 150;

	int y = 0;

	if (Column == 1)
		y = yc1;

	else if (Column == 2)
		y = yc2;

	else
		y = yc3;

	g_Render->text(x, y, Text, Font_Menu, off);

	if ( GetAsyncKeyState(Key) & 1)
	{
		if (selectedInt != iCounter)
			selectedInt = iCounter;

		else
			selectedInt = -1;
	}

	std::string temp = std::to_string(*Var);
	const char* str = temp.c_str();

	if (selectedInt == iCounter)
	{
		g_Render->text(x + 100, y, str, Font_Menu, on);

		if (GetAsyncKeyState(VK_RIGHT) & 1)
		{
			if (*Var == Max)
				*Var = Min;

			else
				*Var += 1;
		}

		if (GetAsyncKeyState(VK_LEFT) & 1)
		{
			if (*Var == Min)
				*Var = Max;

			else
				*Var -= 1;
		}
	}

	else
		g_Render->text(x + 100, y, str, Font_Menu, off);

	if (Column == 1)
		yc1 += 20;

	else if (Column == 2)
		yc2 += 20;

	else
		yc3 += 20;

	if(GetAsyncKeyState(VK_RETURN) & 1)
		selectedInt = -1;
}

void Menu::AddBool(const char* Text, bool* Var, int Key, int Column)
{
	static Color off = Color(255, 255, 255, 255);
	static Color on = Color(50, 150, 250, 255);

	int x = Column * 150;

	int y = 0;

	if (Column == 1)
		y = yc1;

	else if (Column == 2)
		y = yc2;

	else
		y = yc3;

	g_Render->text(x, y, Text, Font_Menu, off);

	if(*Var)
		g_Render->text(x + 100, y, "ON", Font_Menu, on);
	else
		g_Render->text(x + 100, y, "OFF", Font_Menu, off);

	if (GetAsyncKeyState(Key) & 1)
		*Var = !(*Var);

	if (Column == 1)
		yc1 += 20;

	else if (Column == 2)
		yc2 += 20;

	else
		yc3 += 20;
}

void Menu::Render()
{
	iCounter = 0;
	yc1 = 60;
	yc2 = 60;
	yc3 = 60;
	AddBool("Box", &g_Vars.visuals.Box, VK_F1, 1);
	AddBool("Corners", &g_Vars.visuals.Corners, VK_F2, 1);
	AddBool("Outline", &g_Vars.visuals.Outline, VK_F3, 1);
	AddBool("Health", &g_Vars.visuals.Health, VK_F4, 1);
	AddBool("Name", &g_Vars.visuals.Name, VK_F5, 1);
	AddInt("Snaplines", &g_Vars.visuals.Snaplines, 0, 3, VK_F6, 1);
	AddInt("Chams", &g_Vars.visuals.ChamsStyle, 0, 2, VK_F7, 1);
	AddBool("Glow", &g_Vars.visuals.Glow, VK_F8, 1);
	AddInt("FOV", &g_Vars.visuals.ViewModelFov, 0, 2, VK_F9, 1);
	AddInt("Weapon Chams", &g_Vars.visuals.WeaponChams, 0, 2, VK_F10, 1);
	
	AddBool("BackTrack", &g_Vars.legit.Backtrack, VK_NUMPAD7, 2);
	AddBool("Exploit", &g_Vars.misc.FakeLatencyEnable, 0x45, 2);
	AddInt("Exploit", &g_Vars.misc.FakeLatencyMode, 1, 2, VK_NUMPAD4, 2);
	AddInt("Chams", &g_Vars.visuals.BacktrackChams, 0, 2, VK_NUMPAD1, 2);
	AddBool("Dots", &g_Vars.visuals.BacktrackDots, VK_NUMPAD0, 2);
	AddBool("Legit AA", &g_Vars.legit.AA, VK_NUMPAD8, 2);

	AddBool("Bhop", &g_Vars.misc.Bhop, VK_NUMPAD9, 3);
	AddBool("AutoStrafe", &g_Vars.misc.AutoStrafe, VK_NUMPAD6, 3);
}
