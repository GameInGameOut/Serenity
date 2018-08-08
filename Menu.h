#pragma once
#include "SDK.h"

class Menu
{
public:
	void Render();

private:
	void AddBool(const char* Text, bool* Var, int Key, int Column);
	void AddInt(const char* Text, int* Var, int Low, int High, int Key, int Column);
}; extern Menu* g_Menu;