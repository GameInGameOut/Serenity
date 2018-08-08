//#include "LegitBot.h"
//
//float GetFov(const Vector &_from, const Vector &_to)
//{
//	return RAD2DEG(acos(_from.Dot(_to) / _from.LengthSqr()));
//}
//
//int LegitBot::GetClosestBone(C_BaseEntity* ent)
//{
//	float BestDist = 360.f;
//	int aimbone;
//
//	matrix3x4_t matrix[MAXSTUDIOBONES];
//
//	if (!ent->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, i_Engine->GetLastTimeStamp()))
//		return -1;
//
//	studiohdr_t* pStudioModel = i_ModelInfo->GetStudioModel(ent->GetModel());
//	if (!pStudioModel)
//		return -1;
//
//	mstudiohitboxset_t* set = pStudioModel->pHitboxSet(ent->GetHitboxSet());
//	if (!set)
//		return -1;
//
//	for (int i = 0; i < set->numhitboxes; i++)
//	{
//		mstudiobbox_t* hitbox = set->pHitbox(i);
//
//		if (!hitbox)
//			continue;
//
//		auto thisdist = GetFov(g_Globals.cmd->viewangles, Math::CalcAngle(g_Globals.localplayer->GetEyePos(), Vector(matrix[hitbox->bone][0][3], matrix[hitbox->bone][1][3], matrix[hitbox->bone][2][3])));
//
//		if (BestDist > thisdist)
//		{
//			BestDist = thisdist;
//			aimbone = hitbox->bone;
//			continue;
//		}
//	}
//
//	return aimbone;
//}
//
//int LegitBot::GetBestTick(C_BaseEntity* ent, int bone)
//{
//	auto record = f_Backtrack.entityPostionData[ent->GetIndex()];
//	float bestFov = 360;
//	int bestTick = -1;
//
//	Vector localViewAngles = g_Globals.cmd->viewangles + g_Globals.localplayer->GetPunchAngles() * 2.f;
//
//	for (int i = 0; i < 13; i++)
//	{
//		auto matrix = record[i].matrix;
//		mstudiohitboxset_t* hitboxSet = record[i].hitboxSet;
//		mstudiobbox_t* hitbox = hitboxSet->pHitbox(i);
//
//
//		Vector bonePos = Vector(matrix[hitbox->bone][0][3], matrix[hitbox->bone][1][3], matrix[hitbox->bone][2][3]);
//		Vector aimPoint = Math::CalculateAngle(g_Globals.localplayer->GetEyePos(), bonePos);
//
//		float fov = GetFov(localViewAngles, aimPoint);
//
//		if (fov < bestFov)
//		{
//			bestFov = fov;
//			bestTick = i;
//		}
//	}
//
//	return bestTick;
//}
//
//void LegitBot::Run()
//{
//	float closestFov = 360;
//	C_BaseEntity* bestEnt = g_Globals.localplayer;
//
//	Vector localViewAngles = g_Globals.cmd->viewangles + g_Globals.localplayer->GetPunchAngles() * 2.f;
//
//	for (int i = 0; i < i_Engine->GetMaxClients(); i++)
//	{
//		C_BaseEntity* ent = i_EntList->GetClientEntity(i);
//
//		if (!ent || ent->GetHealth() < 1 || ent->GetDormant()) continue;
//
//		Vector targetsHeadPosition = ent->GetHitboxPos(0);
//		Vector aimAngle = Math::CalculateAngle(g_Globals.localplayer->GetEyePos(), targetsHeadPosition);
//
//		float fov = GetFov(localViewAngles, aimAngle);
//
//		if (fov < closestFov)
//		{
//			closestFov = fov;
//			bestEnt = ent;
//		}
//	}
//
//	if (g_Globals.cmd->buttons & IN_ATTACK)
//	{
//		if (!bestEnt || bestEnt == g_Globals.localplayer) 
//			return;
//
//		int bone = GetClosestBone(bestEnt);
//
//		if (bone == -1)
//			return;
//
//		int bestTick = GetBestTick(bestEnt, bone);
//
//		if (bestTick == -1)
//			return;
//
//		i_Engine->SetViewAngles(QAngle(f_Backtrack.entityPostionData[bestEnt->GetIndex()][bestTick].headPosition.x, f_Backtrack.entityPostionData[bestEnt->GetIndex()][bestTick].headPosition.y, f_Backtrack.entityPostionData[bestEnt->GetIndex()][bestTick].headPosition.z));
//	}
//}