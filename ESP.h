#pragma once
#include "SDK.h"

struct Box
{
	int x = -1, y = -1, w = -1, h = -1;
};

class CESP
{
public:
	void Start();
	bool WorldToScreen(const Vector& in, Vector& out);
private:
	void PlayerBox(C_BaseEntity* entity, Box EspRect, Color BoxClr);
	void PlayerHealth(C_BaseEntity* entity, Box EspRect, Color BoxClr);
	void PlayerWeapon(C_BaseEntity* entity, Box EspRect);
	void PlayerMoney(C_BaseEntity* entity, Box EspRect);
	void PlayerName(C_BaseEntity* entity, Box EspRect, player_info_t info);
	void PlayerDistance(C_BaseEntity* entity, Box EspRect,  C_BaseEntity* localplayer);
	void BacktrackTicks(C_BaseEntity* ent, C_BaseEntity* localplayer);
	void ResolverInfo(C_BaseEntity* ent, C_BaseEntity* localplayer);
	void Snaplines(C_BaseEntity* entity, Box EspRect, Color BoxClr, int mode);

	bool GetBox(C_BaseEntity* m_entity, Box& box, bool dynamic);
}; extern CESP* f_Esp;