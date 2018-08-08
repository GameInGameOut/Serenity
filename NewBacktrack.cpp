#include "NewBacktrack.h"

#define TIME_TO_TICKS( deltatime )		( (int)( 0.5f + (float)(deltatime) / g_pGlobalVars->interval_per_tick ) )

#define TICKS_TO_TIME( t )      ( g_pGlobalVars->interval_per_tick * ( t ) )

inline float GetFov(const Vector &_from, const Vector &_to)
{
	return RAD2DEG(acos(_from.Dot(_to) / _from.LengthSqr()));
}

void Backtrack::Run()
{
    for(int i = 0; i < g_pEngine->GetMaxClients(); i++)
    {
        C_BaseEntity* ent = g_pEntityList->GetClientEntity(i);
        
        if(!ent || ent->GetHealth() < 1 || ent->GetDormant()) 
            continue;
        
        
    }
}