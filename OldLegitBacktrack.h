#pragma once
#include "sdk.h"
#include <vector>
#include <deque>

struct BacktrackData
{
	Vector headPos;
	float simTime;
	Vector origin;
	QAngle angs;
};

class Backtracking
{
public:
	bool Run(CUserCmd* _cmd, C_BaseEntity* _localPlayer, int _maxTick);

	int bestTarget = -1;
	std::deque<BacktrackData> btData[64];
}; extern Backtracking f_Backtrack;