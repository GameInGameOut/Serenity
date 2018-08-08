#pragma once
#include "SDK.h"

class RageBot
{
public:
	void Run(CUserCmd* pCmd);
	void RCS(CUserCmd* pCmd);
private:
	Vector GetNewAimpoint(C_BaseEntity* pLocal, C_BaseEntity* pTarget, float& simtime, Vector& origin);
}; extern RageBot* f_Rage;