#include "SkinChanger.h"

void CSkinChanger::Run()
{
	for (int i = 0; i < i_Engine->GetMaxClients(); i++)
	{
		C_BaseWeapon* weapon = (C_BaseWeapon*)i_EntList->GetClientEntity(i);

		if (!weapon)
			continue;

		C_BaseEntity* owner = i_EntList->GetClientEntityFromHandle(weapon->OwnerEntity());

		if (!owner || owner != g_Globals->localplayer)
			continue;
	}
}

Parser::vecSkinInfo CSkinChanger::GetSkins(int gun)
{

}