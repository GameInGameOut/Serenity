#include "Movement.h"

void CMovement::MoveFix(Vector vOldAngles, float fOldForward, float fOldSidemove)
{
	float deltaView;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (g_Globals.cmd->viewangles.y < 0.0f)
		f2 = 360.0f + g_Globals.cmd->viewangles.y;
	else
		f2 = g_Globals.cmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	g_Globals.cmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	g_Globals.cmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

void CMovement::Bhop(C_BaseEntity* localplayer, CUserCmd* cmd) 
{
	if (cmd->buttons & IN_JUMP) 
	{
		if (localplayer->GetFlags() & FL_ONGROUND || localplayer->GetMoveType() & MOVETYPE_LADDER)
		{
			cmd->buttons |= IN_JUMP;
		}
		else
		{
			cmd->buttons &= ~IN_JUMP;
		}
	}
}

bool AutoStrafeFlip = true;

void CMovement::AutoStrafe(C_BaseEntity* localplayer, CUserCmd* cmd)
{
	if (localplayer->GetFlags() & FL_ONGROUND)
		return;

	cmd->forwardmove = 0.0f;
	cmd->sidemove = 0.0f;

	if (cmd->mousedx < 0)
	{
		cmd->sidemove = -450.0f;
	}
	else if (cmd->mousedx > 0)
	{
		cmd->sidemove = 450.0f;
	}
	else
	{
		if (AutoStrafeFlip)
		{
			cmd->viewangles = Math::NormalizeAngle(Vector(cmd->viewangles.x, cmd->viewangles.y - 1.0f, 0.0f));
			cmd->sidemove = -450.0f;
			AutoStrafeFlip = false;
		}
		else
		{
			cmd->viewangles = Math::NormalizeAngle(Vector(cmd->viewangles.x, cmd->viewangles.y + 1.0f, 0.0f));
			cmd->sidemove = 450.0f;
			AutoStrafeFlip = true;
		}
	}
}