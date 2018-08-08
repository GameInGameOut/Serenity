#include "ESP.h"

float dot_product_t(const float* a, const float* b) {
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

void vector_transform_a(const float *in1, const matrix3x4_t& in2, float *out) {
	out[0] = dot_product_t(in1, in2[0]) + in2[0][3];
	out[1] = dot_product_t(in1, in2[1]) + in2[1][3];
	out[2] = dot_product_t(in1, in2[2]) + in2[2][3];
}

inline void vector_transform_z(const Vector& in1, const matrix3x4_t &in2, Vector &out) 
{
	vector_transform_a(&in1.x, in2, &out.x);
}

Color GetHealthCol(C_BaseEntity* ent)
{
	const float  multiplier = 2.55; //number we multiply our health by to get our colors(multiply our health by 2.55 to give a number we then use for the BoxClr. since 255 is the max of any BoxClr for our esp ie. 100 full health * 2.55 = 255 or max BoxClr)

	int red, green, blue, alpha;	//colors we will solve for based on our entities health
	int health = ent->GetHealth();	//get entities health

	red = 255 - (health * multiplier);	//find red value (no health = max red, full health = no red)
	green = health * multiplier;	//find green value (full health = max green, no health = no green)
	blue = 0;	//no blue on BoxClr scale red to green
	alpha = 255;	//max alpha

	Color BoxClr(red, green, blue, alpha);	//create BoxClr and fill it with values
	return BoxClr;	//return BoxClr

}

bool CESP::GetBox(C_BaseEntity* m_entity, Box& box, bool dynamic)
{
	DWORD m_rgflCoordinateFrame = (DWORD)0x470 - 0x30;
	const matrix3x4_t& trnsf = *(matrix3x4_t*)((DWORD)m_entity + (DWORD)m_rgflCoordinateFrame);

	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;

	float left, top, right, bottom;

	vOrigin = m_entity->GetOrigin();

	ICollideable* collideable = m_entity->GetCollideable();


	min = collideable->OBBMins();
	max = collideable->OBBMaxs();

	if (!dynamic) {
		min += vOrigin;
		max += vOrigin;
	}

	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	Vector Vector_transformed[8];

	if (dynamic)
	{
		for (int i = 0; i < 8; i++)
		{
			vector_transform_z(points[i], trnsf, Vector_transformed[i]);
			points[i] = Vector_transformed[i];
		}
	}
	if (!f_Esp->WorldToScreen(points[3], flb) || !f_Esp->WorldToScreen(points[5], brt)
		|| !f_Esp->WorldToScreen(points[0], blb) || !f_Esp->WorldToScreen(points[4], frt)
		|| !f_Esp->WorldToScreen(points[2], frb) || !f_Esp->WorldToScreen(points[1], brb)
		|| !f_Esp->WorldToScreen(points[6], blt) || !f_Esp->WorldToScreen(points[7], flt))
		return false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	box.x = left;
	box.y = top;
	box.w = right - left;
	box.h = bottom - top;

	return true;
}

void CESP::Start() 
{
	for (int i = 0; i <= i_Engine->GetMaxClients(); i++) 
	{
		auto localplayer = i_EntList->GetClientEntity(i_Engine->GetLocalPlayer());
		auto entity = i_EntList->GetClientEntity(i);

		if (!entity->IsValid() || (entity->IsTeam() && g_Vars.visuals.EnemyOnly))
			continue;

		Box espBox;

		if (!GetBox(entity, espBox, true))
			continue;

		player_info_t info;
		//Color BoxClr = entity->GetTeam() == localplayer->GetTeam() ? Color(0, 255, 0, 255) : Color(255, 0, 0, 255);
		Color BoxCol = GetHealthCol(entity);

		i_Engine->GetPlayerInfo(entity->GetIndex(), &info);

		if (g_Vars.visuals.Box)
			PlayerBox(entity, espBox, BoxCol);

		if (g_Vars.visuals.Name)
			PlayerName(entity, espBox, info);

		if (g_Vars.visuals.Weapon)
			PlayerWeapon(entity, espBox);

		if (g_Vars.visuals.Distance)
			PlayerDistance(entity, espBox, localplayer);

		if (g_Vars.visuals.Health)
			PlayerHealth(entity, espBox, BoxCol);

		if (g_Vars.visuals.Money)
			PlayerMoney(entity, espBox);

		if (g_Vars.visuals.BacktrackDots && g_Vars.legit.Backtrack)
			BacktrackTicks(entity, localplayer);

		int snapMode = g_Vars.visuals.Snaplines;

		if(snapMode > 0)
			Snaplines(entity, espBox, BoxCol, snapMode);
	}
}

void CESP::BacktrackTicks(C_BaseEntity* ent, C_BaseEntity* localplayer)
{
	for (int i = 0; i < f_LegitBacktrack.entData[ent->GetIndex()].size(); i++)
	{
		if (!f_LegitBacktrack.entData[ent->GetIndex()].at(i).isTickValid)
			continue;

		Vector thisTick = f_LegitBacktrack.entData[ent->GetIndex()].at(i).headPos;

		Vector screenThisTick;

		if (WorldToScreen(thisTick, screenThisTick))
		{
			g_Render->rect(screenThisTick.x, screenThisTick.y, 3, 3, Color(255, 255, 255, 200));
		}
	}
}

void CESP::Snaplines(C_BaseEntity* entity, Box EspRect, Color BoxClr, int mode)
{
	int width = 0;
	int height = 0;

	Vector to = Vector(EspRect.x + EspRect.w / 2, EspRect.y + EspRect.h, 0);

	i_Engine->GetScreenSize(width, height);

	Vector From(0,0,0);

	if(mode == 1)
		From = Vector((width / 2), height, 0);

	else if (mode == 2)
		From = Vector((width / 2), (height / 2), 0);

	else
		From = Vector((width / 2), 0, 0);
		
	g_Render->line(From.x, From.y, to.x, to.y, BoxClr);
}

void CESP::PlayerBox(C_BaseEntity* entity, Box EspRect, Color BoxClr)
{	
	if (!g_Vars.visuals.Corners)
	{
		g_Render->outline(EspRect.x, EspRect.y, EspRect.w, EspRect.h, BoxClr);

		if (g_Vars.visuals.Outline) 
		{
			g_Render->outline(EspRect.x - 1, EspRect.y - 1, EspRect.w + 2, EspRect.h + 2, Color(18, 18, 18, 255));
			g_Render->outline(EspRect.x + 1, EspRect.y + 1, EspRect.w - 2, EspRect.h - 2, Color(18, 18, 18, 255));
		}
	}

	else
	{
		float width_corner = EspRect.w / 4;
		float height_corner = width_corner;

		if (g_Vars.visuals.Outline) 
		{
			g_Render->rect(EspRect.x - 1, EspRect.y - 1, width_corner + 2, 3, Color(18, 18, 18, 255));
			g_Render->rect(EspRect.x - 1, EspRect.y - 1, 3, height_corner + 2, Color(18, 18, 18, 255));

			g_Render->rect((EspRect.x + EspRect.w) - width_corner - 1, EspRect.y - 1, width_corner + 2, 3, Color(18, 18, 18, 255));
			g_Render->rect(EspRect.x + EspRect.w - 1, EspRect.y - 1, 3, height_corner + 2, Color(18, 18, 18, 255));

			g_Render->rect(EspRect.x - 1, EspRect.y + EspRect.h - 4, width_corner + 2, 3, Color(18, 18, 18, 255));
			g_Render->rect(EspRect.x - 1, (EspRect.y + EspRect.h) - height_corner - 4, 3, height_corner + 2, Color(18, 18, 18, 255));

			g_Render->rect((EspRect.x + EspRect.w) - width_corner - 1, EspRect.y + EspRect.h - 4, width_corner + 2, 3, Color(18, 18, 18, 255));
			g_Render->rect(EspRect.x + EspRect.w - 1, (EspRect.y + EspRect.h) - height_corner - 4, 3, height_corner + 3, Color(18, 18, 18, 255));
		}

		g_Render->rect(EspRect.x, EspRect.y, width_corner, 1, BoxClr);
		g_Render->rect(EspRect.x, EspRect.y, 1, height_corner, BoxClr);

		g_Render->rect((EspRect.x + EspRect.w) - width_corner, EspRect.y, width_corner, 1, BoxClr);
		g_Render->rect(EspRect.x + EspRect.w, EspRect.y, 1, height_corner, BoxClr);

		g_Render->rect(EspRect.x, EspRect.y + EspRect.h - 3, width_corner, 1, BoxClr);
		g_Render->rect(EspRect.x, (EspRect.y + EspRect.h) - height_corner - 3, 1, height_corner, BoxClr);

		g_Render->rect((EspRect.x + EspRect.w) - width_corner, EspRect.y + EspRect.h - 3, width_corner, 1, BoxClr);
		g_Render->rect(EspRect.x + EspRect.w, (EspRect.y + EspRect.h) - height_corner - 3, 1, height_corner + 1, BoxClr);
	}
}

void CESP::PlayerHealth(C_BaseEntity* entity, Box EspRect, Color BoxClr) 
{
	int player_health = entity->GetHealth() > 100 ? 100 : entity->GetHealth();

	int health_height = player_health * EspRect.h / 100;
	int add_space = EspRect.h - health_height;

	if (g_Vars.visuals.Outline)
		g_Render->outline(EspRect.x - 7, EspRect.y - 1, 4, EspRect.h + 2, Color(21, 21, 21, 255));

	g_Render->rect(EspRect.x - 6, EspRect.y, 2, EspRect.h, Color(21, 21, 21, 255));
	g_Render->rect(EspRect.x - 6, EspRect.y + add_space, 2, health_height, BoxClr);
}

void CESP::PlayerWeapon(C_BaseEntity* entity, Box EspRect)
{
	auto weapon = entity->GetActiveWeapon();
	auto weapondata = weapon->GetCSWeaponData();


	std::string a = std::to_string(weapon->Clip1());
	std::string aa = std::to_string(weapondata->max_clip);
	char const *inClip = a.c_str();
	char const *max = aa.c_str();

	g_Render->text(EspRect.x + EspRect.w + 4, EspRect.y - 5, ("%s[%d/%d]", weapon->GetWeaponName().c_str(), inClip, max), Font_ESP, Color(255, 255, 255, 255));
}

void CESP::PlayerName(C_BaseEntity* entity, Box EspRect,player_info_t info) 
{
	std::string name = info.szName;
	g_Render->text(EspRect.x + EspRect.w + 4, EspRect.y, name.c_str(), Font_ESP, Color(255, 255, 255, 255));
}

void CESP::PlayerMoney(C_BaseEntity* entity, Box EspRect) 
{
	std::string s = std::to_string(entity->GetMoney());
	char const *money = s.c_str();

	g_Render->text(EspRect.x + EspRect.w + 4, EspRect.y + 10, ("%d$", money), Font_ESP, Color(0, 222, 0, 255));
}

void CESP::PlayerDistance(C_BaseEntity* entity, Box EspRect, C_BaseEntity* localplayer) {
	auto a = localplayer->GetOrigin();
	auto b = entity->GetOrigin();
	int dist = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)) * 0.0254f;
	std::string s = std::to_string(dist);
	char const *distance = s.c_str();

	g_Render->text(EspRect.x + EspRect.w + 4, EspRect.y + 20, ("%dM", distance), Font_ESP, Color(255, 255, 255, 255));
}

bool CESP::WorldToScreen(const Vector& in, Vector& out) 
{
	static ptrdiff_t ptrViewMatrix;
	if (!ptrViewMatrix)
	{
		ptrViewMatrix = static_cast<ptrdiff_t>(g_Utils->FindPatternIDA("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
		ptrViewMatrix += 0x3;
		ptrViewMatrix = *reinterpret_cast<uintptr_t*>(ptrViewMatrix);
		ptrViewMatrix += 176;
	}
	const matrix3x4_t& worldToScreen = *(matrix3x4_t*)ptrViewMatrix;

	int ScrW, ScrH;

	i_Engine->GetScreenSize(ScrW, ScrH);

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3];
	out.z = 0;
	if (w > 0.01)
	{
		float inverseWidth = 1 / w;
		out.x = (ScrW / 2) + (0.5 * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * inverseWidth) * ScrW + 0.5);
		out.y = (ScrH / 2) - (0.5 * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * inverseWidth) * ScrH + 0.5);
		return true;
	}
	return false;
}