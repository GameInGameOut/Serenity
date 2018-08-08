#pragma once
#include "SDK.h"
class CMovement {
public:
	void MoveFix(Vector vOldAngles, float fOldForward, float fOldSidemove);
	void Bhop(C_BaseEntity* localplayer, CUserCmd* cmd);
	void AutoStrafe(C_BaseEntity* localplayer, CUserCmd* cmd);
}; extern CMovement* f_Movement;