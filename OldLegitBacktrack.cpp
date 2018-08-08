#include "LegitBacktrack.h"
#include "ESP.h"

#define TICK_INTERVAL			(g_pGlobalVars->interval_per_tick)
 
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

#define TICKS_TO_TIME( t )      ( g_pGlobalVars->interval_per_tick * ( t ) )

template <class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float DistPointToLine(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

Vector AngleVector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}

float GetEstimatedServerTime()
{
	return Globals::pLocal->GetTickBase() * TICK_INTERVAL;
}

ConVar* minupdate;
ConVar* maxupdate;
ConVar* updaterate;
ConVar* interprate;
ConVar* cmin;
ConVar* cmax;
ConVar* interp;

float GetLerpTime()
{
	if (!minupdate)
		minupdate = g_pICvar->FindVar(("sv_minupdaterate"));
	if (!maxupdate)
		maxupdate = g_pICvar->FindVar(("sv_maxupdaterate"));
	if (!updaterate)
		updaterate = g_pICvar->FindVar(("cl_updaterate"));
	if (!interprate)
		interprate = g_pICvar->FindVar(("cl_interp_ratio"));
	if (!cmin)
		cmin = g_pICvar->FindVar(("sv_client_min_interp_ratio"));
	if (!cmax)
		cmax = g_pICvar->FindVar(("sv_client_max_interp_ratio"));
	if (!interp)
		interp = g_pICvar->FindVar(("cl_interp"));

	float UpdateRate = updaterate->GetFloat();
	float LerpRatio = interprate->GetFloat();

	return max(LerpRatio / UpdateRate, interp->GetFloat());
}

bool Backtracking::Run(CUserCmd* cmd, C_BaseEntity* pLocal, int _maxTick)
{
	int bestTargetIndex = -1;
	int maxtick = 13;

	if(g_Vars.misc.FakeLatencyEnable)
		maxtick = 64;

	float bestFov = FLT_MAX;
	player_info_t info;
	if (pLocal->GetHealth() < 1)
		return false;

	for (int i = 0; i < g_pEngine->GetMaxClients(); i++)
	{
		C_BaseEntity* entity = g_pEntityList->GetClientEntity(i);

		if (!entity || !pLocal || entity == pLocal || !g_pEngine->GetPlayerInfo(i, &info) || entity->GetDormant() || entity->GetTeam() == pLocal->GetTeam() || pLocal->GetHealth() < 1)
			continue;


		float simtime = entity->SimTime();
		Vector hitboxPos = entity->GetBonePos(8);
		//this->entityPostionData[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };

		this->btData[i].push_front(BacktrackData{ hitboxPos, simtime, entity->GetOrigin(), entity->GetAngles() });

		while (this->btData[i].size() > maxtick)
		{
			this->btData[i].pop_back();
		}

		Vector ViewDir = AngleVector(cmd->viewangles + (pLocal->GetPunchAngles() * 2.f));
		float FOVDistance = DistPointToLine(hitboxPos, pLocal->GetEyePos(), ViewDir);

		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			bestTargetIndex = i;
		}

	}

	float bestTargetSimTime;
	if (bestTargetIndex != -1)
	{
		float tempFloat = FLT_MAX;
		Vector ViewDir = AngleVector(cmd->viewangles + (pLocal->GetPunchAngles() * 2.f));
		for (int t = this->btData[bestTargetIndex].size() - g_Vars.legit.BacktrackTicks; t < this->btData[bestTargetIndex].size(); ++t)
		{
			float tempFOVDistance = DistPointToLine(this->btData[bestTargetIndex].at(t).headPos, pLocal->GetEyePos(), ViewDir);
			if (tempFloat > tempFOVDistance && this->btData[bestTargetIndex].at(t).simTime > pLocal->SimTime() - 1)
			{
				tempFloat = tempFOVDistance;
				bestTargetSimTime = this->btData[bestTargetIndex].at(t).simTime;
			}
		}
		if (cmd->buttons & IN_ATTACK)
		{
			int bestTickTime = TIME_TO_TICKS(bestTargetSimTime);

			g_pUtils->Print("Backtracking from tickcount %d To %d, a total of %d ticks", cmd->tick_count, bestTickTime, cmd->tick_count - bestTickTime);

			cmd->tick_count = bestTickTime;
		}
	}
	return true;
}


float GetFOV(const Vector &_from, const Vector &_to)
{
	return RAD2DEG(acos(_from.Dot(_to) / _from.LengthSqr()));
}

Backtracking f_Backtrack;