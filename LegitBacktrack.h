#pragma once

#include <deque>
#include "sdk.h"

struct EntityData
{
	Vector headPos;
	float simulationTime;
	Vector origin;
	QAngle angs;
	bool isTickValid;
};

class LegitBacktrack
{
public:
	void Run();

	std::deque<EntityData> entData[64];
private:
	bool CheckValidTick(int tick, EntityData data, bool fakeLatency);
	void SetInvalidTicks(C_BaseEntity* ent, std::deque<EntityData>& data, bool fakeLatency);
	void UpdateEntityData(C_BaseEntity* ent, bool fakeLatency);
	C_BaseEntity* GetClosestEntity();
	void BacktrackEntity(C_BaseEntity* ent);
}; extern LegitBacktrack f_LegitBacktrack;