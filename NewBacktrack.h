#pragma once

#include <deque>
#include "sdk.h"

struct EntityData
{
	Vector headPosition;
	float simulationTime;
	Vector origin;
	QAngle angs;
};

class Backtracking
{
public:
	void Run();
private:
    void CheckValidTick(int tick, bool fakeLatency);
	void UpdateEntityData(C_BaseEntity* ent);
	void BacktrackEntity(C_BaseEntity* ent);
	
	std::deque<EntityData> entData[64];
}; extern Backtracking f_Backtrack;