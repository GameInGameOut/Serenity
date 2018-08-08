#include "Glow.h"

void CGlow::Start() 
{
	auto GlowObjectManager = f_GlowManager;
	CGlowObjectManager::GlowObjectDefinition_t* glowEntity;
	auto local = i_EntList->GetClientEntity(i_Engine->GetLocalPlayer());

	for (int i = 0; i < GlowObjectManager->size; i++)
	{
		glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		auto entity = glowEntity->getEntity();

		if (!entity->IsValid() || (entity->IsTeam() && g_Vars.visuals.EnemyOnly))
			continue;

		auto EntityClass = entity->GetClientClass();
		if (EntityClass->m_ClassID == ClassId_CCSPlayer) {
			Color cglow;
			cglow = entity->GetTeam() == local->GetTeam() ? Color(0, 255, 0, 180) : Color(255, 0, 0, 180);
			glowEntity->set(cglow);
		}
	}
}