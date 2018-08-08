#include "Misc.h"

void CMisc::LegitAA(bool &bSendPacket)
{
	auto cmd = g_Globals.cmd;

	Vector oldAngle = cmd->viewangles;
	float oldForward = cmd->forwardmove;
	float oldSideMove = cmd->sidemove;
	if (cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK)
		return;

	auto pWeapon = g_Globals.localplayer->GetActiveWeapon();
	if (!pWeapon || !pWeapon->IsGun())
		return;

	static int ChokedPackets = -1;
	if (1 > ChokedPackets) {
		bSendPacket = false;
		static bool dir = false;
		static bool dir2 = false;
		dir2 = false;
		if (cmd->sidemove > 1) dir = true;
		else if (cmd->sidemove < -1) dir = false;
		cmd->viewangles.y = (dir) ? (cmd->viewangles.y - 90) : (cmd->viewangles.y - 180) - 90.f;
		ChokedPackets++;
	}
	else
	{
		bSendPacket = true;
		ChokedPackets = -1;
	}
	Math::NormalizeAngle(cmd->viewangles);
	Math::ClampAngles(cmd->viewangles);
} CMisc* f_Misc;