#pragma once
#include "SDK.h"

struct LegitBot
{
private:
	
	int GetClosestBone(C_BaseEntity* ent);
	int GetBestTick(C_BaseEntity* ent, int bone);

public:
	void Run();
};