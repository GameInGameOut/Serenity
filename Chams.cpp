#include "Chams.h"

void CChams::Start()
{
	if (!i_Engine->IsConnected() && !i_Engine->IsInGame())
		return;

	static IMaterial* nignorez = i_MatSystem->FindMaterial("reflective_ignorez", TEXTURE_GROUP_MODEL);
	static IMaterial* nnormal = i_MatSystem->FindMaterial("reflective_normal", TEXTURE_GROUP_MODEL);
	static IMaterial* fignorez = i_MatSystem->FindMaterial("flat_ignorez", TEXTURE_GROUP_MODEL);
	static IMaterial* fnormal = i_MatSystem->FindMaterial("flat_normal", TEXTURE_GROUP_MODEL);

	int chamsType = g_Vars.visuals.ChamsStyle;

	IMaterial* ignorez = nignorez;
	IMaterial* normal = nnormal;
	
	if (chamsType == 2)
	{
		ignorez = fignorez;
		normal = fnormal;
	}

	auto localplayer = i_EntList->GetClientEntity(i_Engine->GetLocalPlayer());

	for (int i = 0; i <= i_Engine->GetMaxClients(); i++)
	{
		C_BaseEntity* ent = i_EntList->GetClientEntity(i);

		if (!ent->IsValid())
			continue;

		if (g_Vars.visuals.EnemyOnly && ent->GetTeam() == localplayer->GetTeam()) continue;

		

		if (g_Vars.visuals.ChamsStyle == 0)
		{
			i_ModelRender->ForcedMaterialOverride(nullptr);
			continue;
		}
				
		i_RenderView->SetBlend(1);

		if (!g_Vars.visuals.ChamsVisOnly)
		{
			i_RenderView->SetColorModulation(1, 0, 0);
			i_ModelRender->ForcedMaterialOverride(ignorez);
			ent->DrawModel(0x1, 255);
		}

		i_RenderView->SetColorModulation(0, 1, 1);
		i_ModelRender->ForcedMaterialOverride(normal);
		ent->DrawModel(0x1, 255);
	}

	i_ModelRender->ForcedMaterialOverride(nullptr);
}

void CChams::BacktrackChams()
{
	static IMaterial* mat = i_MatSystem->FindMaterial("reflective_normal", TEXTURE_GROUP_MODEL);

	mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);

	for (int i = 0; i < i_Engine->GetMaxClients(); i++)
	{
		C_BaseEntity* ent = i_EntList->GetClientEntity(i);

		if (!ent->IsValid() || ent->IsTeam() || f_LegitBacktrack.entData[ent->GetIndex()].size() < 1)
			continue;

		i_RenderView->SetColorModulation(0.5, 1, 0);
		i_ModelRender->ForcedMaterialOverride(mat);

		Vector oldOrigin = ent->GetOrigin();
		QAngle oldAngs = ent->GetAngles();

		if (g_Vars.visuals.BacktrackChams == 1)
		{
			for (int i = 1; i < f_LegitBacktrack.entData[ent->GetIndex()].size(); i++)
			{
				if (!f_LegitBacktrack.entData[ent->GetIndex()].at(i).isTickValid || i > f_LegitBacktrack.entData[ent->GetIndex()].size())
					continue;

				auto record = f_LegitBacktrack.entData[ent->GetIndex()].at(i);
				ent->SetOrigin(record.origin);
				ent->SetAngles(record.angs);
				Vector temp = ent->GetOrigin();
				i_RenderView->SetBlend(0.35);
				ent->DrawModel(0x1, 255);
			}

			ent->SetAngles(oldAngs);
			ent->SetOrigin(oldOrigin);
		}

		else
		{
			auto record = f_LegitBacktrack.entData[ent->GetIndex()].back();
			ent->SetOrigin(record.origin);
			ent->SetAngles(record.angs);
			Vector temp = ent->GetOrigin();
			i_RenderView->SetBlend(0.35);
			ent->DrawModel(0x1, 255);

			ent->SetAngles(oldAngs);
			ent->SetOrigin(oldOrigin);
		}
	}
}