#include "LegitBacktrack.h"

Vector AngleVector(Vector vec)
{
	auto sy = sin(vec.y / 180.f * static_cast<float>(PI));
	auto cy = cos(vec.y / 180.f * static_cast<float>(PI));

	auto sp = sin(vec.x / 180.f * static_cast<float>(PI));
	auto cp = cos(vec.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}

float clamp( float val, float minVal, float maxVal )
{
	if ( maxVal < minVal )
		return maxVal;
	else if( val < minVal )
		return minVal;
	else if( val > maxVal )
		return maxVal;
	else
		return val;
}

float TicksToTime(int tick)
{
	return i_GlobalVars->interval_per_tick * tick;
}

int TimeToTicks(float deltaTime)
{
	return ((int)0.5f + deltaTime / i_GlobalVars->interval_per_tick);
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
		minupdate = i_Cvar->FindVar(("sv_minupdaterate"));
	if (!maxupdate)
		maxupdate = i_Cvar->FindVar(("sv_maxupdaterate"));
	if (!updaterate)
		updaterate = i_Cvar->FindVar(("cl_updaterate"));
	if (!interprate)
		interprate = i_Cvar->FindVar(("cl_interp_ratio"));
	if (!cmin)
		cmin = i_Cvar->FindVar(("sv_client_min_interp_ratio"));
	if (!cmax)
		cmax = i_Cvar->FindVar(("sv_client_max_interp_ratio"));
	if (!interp)
		interp = i_Cvar->FindVar(("cl_interp"));

	auto updateRate = updaterate->GetInt();
	auto interpRatio = static_cast< float >(interprate->GetInt());
	auto minInterpRatio = cmin->GetFloat();
	auto maxInterpRatio = cmax->GetFloat();
	auto minUpdateRate = static_cast< float >(minupdate->GetInt());
	auto maxUpdateRate = static_cast< float >(maxupdate->GetInt());
 
	auto clampedUpdateRate = clamp(updateRate, minUpdateRate, maxUpdateRate);
	auto clampedInterpRatio = clamp(interpRatio, minInterpRatio, maxInterpRatio);
 
	auto lerp = clampedInterpRatio / clampedUpdateRate;
 
	if (lerp <= interprate->GetFloat())
		lerp = interprate->GetFloat();
 
	return lerp;
}

float GetFov(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

void LegitBacktrack::Run()
{
	bool fakeLatency = g_Vars.misc.FakeLatencyEnable;
	int recordSize = fakeLatency ? 64 : 13;

    for(int i = 0; i < i_Engine->GetMaxClients(); i++)
    {
        C_BaseEntity* ent = i_EntList->GetClientEntity(i);
        
        if(!ent->IsValid() || ent->IsTeam()) 
            continue;
        
		UpdateEntityData(ent, fakeLatency);

		while (this->entData[i].size() > recordSize)
		{
			this->entData[i].pop_back();
		}
		SetInvalidTicks(ent, this->entData[i], fakeLatency);
    }

	if (g_Globals.cmd->buttons & IN_ATTACK || g_Globals.cmd->buttons & IN_ATTACK2)
	{
		C_BaseEntity* ent = GetClosestEntity();

		if (!ent)
			return;

		BacktrackEntity(ent);
	}
}

C_BaseEntity* LegitBacktrack::GetClosestEntity()
{
	float fov = 180.f;
	int bestEntIndex = -1;

	Vector pLocalHead = g_Globals.localplayer->GetEyePos();
	Vector viewAngles = AngleVector(g_Globals.cmd->viewangles + (g_Globals.localplayer->GetPunchAngles() * 2.f));

	for (int i = 0; i < i_Engine->GetMaxClients(); i++)
	{
		C_BaseEntity* thisEnt = i_EntList->GetClientEntity(i);

		if (!thisEnt->IsValid() || thisEnt->IsTeam() || this->entData[i].size() < 1)
			continue;

		Vector headPos = this->entData[i].back().headPos;

		float thisFov = GetFov(headPos, pLocalHead, viewAngles);

		if (thisFov < fov)
		{
			fov = thisFov;
			bestEntIndex = i;
		}
	}

	if (bestEntIndex == -1)
	{
		g_Utils->Print("Error! Couldn't get the closest entity to backtrack. Is fov > 180?");
		return nullptr;
	}

	return i_EntList->GetClientEntity(bestEntIndex);
}

void LegitBacktrack::BacktrackEntity(C_BaseEntity* ent)
{
	Vector viewAngles = AngleVector(g_Globals.cmd->viewangles + (g_Globals.localplayer->GetPunchAngles() * 2.f));
	Vector pLocalHead = g_Globals.localplayer->GetEyePos();
	EntityData bestData = this->entData[ent->GetIndex()].back();

	std::deque<EntityData>& data = this->entData[ent->GetIndex()];
	float fov = 180.f;

	for (int i = 0; i < data.size(); i++)
	{
		if (!data.at(i).isTickValid)
			continue;

		Vector headPos = data.at(i).headPos;

		float thisFov = GetFov(headPos, pLocalHead, viewAngles);

		if (thisFov < fov)
		{
			fov = thisFov;
			bestData = data.at(i);
		}
	}

	int bestTickTime = TimeToTicks(bestData.simulationTime);

	g_Utils->Print("Backtracking from tickcount %d To %d, a total of %d ticks", g_Globals.cmd->tick_count, bestTickTime, g_Globals.cmd->tick_count - bestTickTime);

	g_Globals.cmd->tick_count = bestTickTime;
}

ConVar* maxUnlag;

bool LegitBacktrack::CheckValidTick(int tick, EntityData data, bool fakeLatency)
{
	if (!fakeLatency)
		return true;

	float correct = 0;

	if (!maxUnlag)
		maxUnlag = i_Cvar->FindVar("sv_maxunlag");
 
	correct += i_Engine->GetNetChannelInfo()->GetLatency( FLOW_OUTGOING );
	correct += i_Engine->GetNetChannelInfo()->GetLatency( FLOW_INCOMING );
	//correct += GetLerpTime();
	
	correct = clamp(correct, 0, maxUnlag->GetFloat());
 
	float deltaTime = correct - ( i_GlobalVars->curtime - data.simulationTime );

	float maxDelta = 0.2f;

	/*if (fakeLatency)
		maxDelta += 0.8 - i_GlobalVars->interval_per_tick;*/

	bool isValid = false;

	float debug = fabsf(deltaTime);

	

	return fabsf(deltaTime) <= maxDelta; //fabsf

	/*float outlatency;
	INetChannelInfo *nci = i_Engine->GetNetChannelInfo();
	if (nci) {
		outlatency = nci->GetLatency(FLOW_OUTGOING);
	}
	float correct = clamp(outlatency + GetLerpTime(), 0.0f, 1.0f);
	float flTargetTime = TicksToTime(tick);
	float deltaTime = correct - (i_GlobalVars->curtime - flTargetTime);
 
	float flMaxDelta = 0.2f;
	if (fakeLatency)
		flMaxDelta += 0.8 - i_GlobalVars->interval_per_tick;
	return (fabsf(deltaTime) <= flMaxDelta);*/
}

void LegitBacktrack::SetInvalidTicks(C_BaseEntity* ent, std::deque<EntityData>& data, bool fakeLatency)
{
	for (int i = 0; i < data.size(); i++)
	{
		bool isValid = CheckValidTick(i, data.at(i), fakeLatency);

		data.at(i).isTickValid = isValid;
	}
}

void LegitBacktrack::UpdateEntityData(C_BaseEntity* ent, bool fakeLatency)
{
	Vector headPos = ent->GetBonePos(8);
	float simTime = ent->GetSimTime();

	Vector origin = ent->GetOrigin();
	QAngle absAng = ent->GetAngles();

	bool validTick = !fakeLatency;
	
	this->entData[ent->GetIndex()].push_front({ headPos, simTime, origin, absAng, validTick });
}

LegitBacktrack f_LegitBacktrack;