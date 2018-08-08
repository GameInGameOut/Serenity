#include "CBaseEntity.h"
using namespace VT;

bool C_BaseEntity::IsValid()
{
	if (!this || this->GetHealth() < 1 || this->GetDormant() || this == g_Globals.localplayer)
		return false;

	return true;
}

bool C_BaseEntity::IsTeam()
{
	if (this->GetTeam() == g_Globals.localplayer->GetTeam())
		return true;

	return false;
}

bool C_BaseEntity::IsPlayer()
{
	return VFunc<bool(__thiscall*)(C_BaseEntity*)>(this, 152)(this);
}

bool C_BaseEntity::IsWeapon()
{
	return VFunc<bool(__thiscall*)(C_BaseEntity*)>(this, 160)(this);
}
Vector C_BaseEntity::GetViewOffset()
{
	return *reinterpret_cast<Vector*>((uintptr_t)this + NetVars.m_vecViewOffset);
}

int C_BaseEntity::GetHealth()
{
	return *reinterpret_cast<int*>((uintptr_t)this + NetVars.m_iHealth);
}
int C_BaseEntity::GetTeam()
{
	if (!this)
		return 0;

	return *reinterpret_cast<int*>((uintptr_t)this + NetVars.m_iTeamNum);
}
bool C_BaseEntity::GetDormant()
{
	return *reinterpret_cast<bool*>((uintptr_t)this + 0xE9);
}
ICollideable* C_BaseEntity::GetCollideable()
{
	typedef ICollideable*(__thiscall *OrigFn)(void*);
	return VFunc<OrigFn>(this, 3)(this);
}

const model_t* C_BaseEntity::GetModel()
{
	void *pRenderable = (void*)(this + 0x4);
	typedef const model_t*(__thiscall *OrigFn)(void*);
	return VFunc<OrigFn>(pRenderable, 8)(pRenderable);
}

bool C_BaseEntity::SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	void *pRenderable = (void*)(this + 0x4);
	typedef bool(__thiscall *OrigFn)(void*, matrix3x4_t*, int, int, float);
	return VFunc<OrigFn>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}

ClientClass* C_BaseEntity::GetClientClass()
{
	void *pNetworked = (void*)(this + 0x8);
	typedef ClientClass*(__thiscall *OrigFn)(void*);
	return VFunc<OrigFn>(pNetworked, 2)(pNetworked);
}
int C_BaseEntity::GetIndex()
{
	return *reinterpret_cast<int*>((uintptr_t)this + 0x64);
}
int C_BaseEntity::GetFlags()
{
	return *reinterpret_cast<int*>((uintptr_t)this + NetVars.m_fFlags);
}
float C_BaseEntity::GetSimTime()
{
	return *reinterpret_cast<float*>((uintptr_t)this + NetVars.m_flSimulationTime);
}
std::array<float, 24>& C_BaseEntity::m_flPoseParameter()
{
	static int _m_flPoseParameter = NetVars.m_flPoseParameter;
	return *(std::array<float, 24>*)((uintptr_t)this + _m_flPoseParameter);
}
Vector C_BaseEntity::GetOrigin()
{
	return *reinterpret_cast<Vector*>((uintptr_t)this + NetVars.m_vecOrigin);
}

void C_BaseEntity::SetOrigin(Vector origin)
{
	using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
	static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)g_Utils->FindPatternIDA("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
	
	SetAbsOrigin(this, origin);
}

void C_BaseEntity::SetAngles(const QAngle &angles)
{
	using SetAbsAnglesFn = void(__thiscall*)(void*, const QAngle &angles);
	static SetAbsAnglesFn SetAbsAngles = (SetAbsAnglesFn)g_Utils->FindPatternIDA("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");
	
	SetAbsAngles(this, angles);
}

QAngle& C_BaseEntity::GetAngles()
{
	typedef QAngle&(__thiscall* oGetAbsAngles)(PVOID);
	return VFunc< oGetAbsAngles >(this, 11)(this);
}

const matrix3x4_t& C_BaseEntity::GetCoordinateFrame()
{
	return *reinterpret_cast<matrix3x4_t*>((uintptr_t)this + NetVars.m_rgflCoordinateFrame);
}
Vector C_BaseEntity::GetEyePos()
{
	return (this->GetOrigin() + this->GetViewOffset());
}

float C_BaseEntity::NextAttack()
{
	return *reinterpret_cast<float*>(uintptr_t(this) + 0x2D60);
}

bool C_BaseEntity::HasHelmet()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) + 0xB22C);
}

Vector C_BaseEntity::GetVelocity()
{
	return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x110);
}

QAngle* C_BaseEntity::EyeAngs()
{
	return reinterpret_cast<QAngle*>((uintptr_t)this + NetVars.m_angEyeAngles);
}

float C_BaseEntity::Lby()
{
	return *reinterpret_cast<float*>((uintptr_t)this + NetVars.m_flLowerBodyYawTarget);
}

AnimationLayer& C_BaseEntity::GetAnimOverlay(int Index)
{
	return (*(AnimationLayer**)((DWORD)this + 0x2970))[Index];
}

AnimationLayer* C_BaseEntity::GetAnimOverlays()
{
	return *(AnimationLayer**)((DWORD)this + 10608);
}

int C_BaseEntity::GetSequence()
{
	return *reinterpret_cast<int*>((DWORD)this + NetVars.m_nSequence);
}

int C_BaseEntity::GetNumAnimOverlays()
{
	return *(int*)((DWORD)this + 0x297C);
}

float C_BaseEntity::GetAnimationTime()
{
	return *reinterpret_cast<float*>((DWORD)this + 0x3C);
}

float C_BaseEntity::GetCycle()
{
	return *reinterpret_cast<int*>((DWORD)this + NetVars.m_flCycle);
}

int C_BaseEntity::GetSequenceActivity(int sequence)
{
	const auto model = GetModel();
	if (!model)
		return -1;

	const auto hdr = i_ModelInfo->GetStudioModel(model);
	if (!hdr)
		return -1;

	static auto offset = (DWORD)g_Utils->FindPatternIDA("client_panorama.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74 3D");
	static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(offset);

	return GetSequenceActivity(this, hdr, sequence);
}


matrix3x4_t C_BaseEntity::GetBoneMatrix(int BoneID)
{
	matrix3x4_t matrix;

	auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x2698);

	if (offset)
		matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

	return matrix;
}

Vector C_BaseEntity::GetHitboxPos(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		auto studio_model = i_ModelInfo->GetStudioModel(GetModel());
		if (studio_model) {
			auto hitbox = studio_model->pHitboxSet(0)->pHitbox(hitbox_id);
			if (hitbox) {
				auto
					min = Vector{},
					max = Vector{};

				Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
				Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}
	return Vector{};
}
bool C_BaseEntity::GetHitboxPos(int hitbox, Vector &output)
{
	if (hitbox >= HITBOX_MAX)
		return false;

	const model_t *model = this->GetModel();
	if (!model)
		return false;

	studiohdr_t *studioHdr = i_ModelInfo->GetStudioModel(model);
	if (!studioHdr)
		return false;

	matrix3x4_t matrix[MAXSTUDIOBONES];
	if (!this->SetupBones(matrix, MAXSTUDIOBONES, 0x100, 0))
		return false;

	mstudiobbox_t *studioBox = studioHdr->pHitboxSet(0)->pHitbox(hitbox);
	if (!studioBox)
		return false;

	Vector min, max;

	Math::VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
	Math::VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

	output = (min + max) * 0.5f;

	return true;
}
Vector C_BaseEntity::GetBonePos(int bone)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
		return Vector(boneMatrix[bone][0][3], boneMatrix[bone][1][3], boneMatrix[bone][2][3]);
	}
	return Vector{};
}

bool C_BaseEntity::CanSeePlayer(C_BaseEntity* player, int hitbox)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	auto endpos = player->GetHitboxPos(hitbox);

	ray.Init(GetEyePos(), endpos);
	i_Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}


bool C_BaseEntity::CanSeePlayer(C_BaseEntity* player, const Vector& pos)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	auto start = GetEyePos();
	auto dir = (pos - start).Normalized();

	ray.Init(start, pos);
	i_Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}
bool C_BaseEntity::bSpotted()
{
	return *reinterpret_cast<bool*>((uintptr_t)this + 0x939);
}
int C_BaseEntity::GetArmor()
{
	return *reinterpret_cast<int*>((uintptr_t)this + NetVars.m_ArmorValue);
}
int C_BaseEntity::DrawModel(int flags, uint8_t alpha) {
	typedef bool(__thiscall *OrigFn)(void*, int, uint8_t);
	return VFunc<OrigFn>(GetClientRenderable(), 9)(GetClientRenderable(), flags, alpha);
}
void* C_BaseEntity::GetClientRenderable()
{
	return reinterpret_cast<void*>(this + 0x4);
}
int C_BaseEntity::GetTickBase()
{
	return *reinterpret_cast<int*>((uintptr_t)this + NetVars.m_nTickBase);
}
Vector C_BaseEntity::GetPunchAngles()
{
	return *reinterpret_cast<Vector*>((uintptr_t)this + NetVars.m_vecPunchAngles);
}
QAngle C_BaseEntity::QAngPunch()
{
	return *(QAngle*)((DWORD)this + NetVars.m_vecPunchAngles);
}
CBaseHandle C_BaseEntity::hActiveWeapon()
{
	return *reinterpret_cast<CBaseHandle*>((uintptr_t)this + NetVars.m_hActiveWeapon);
}
C_BaseWeapon* C_BaseEntity::GetActiveWeapon()
{
	return (C_BaseWeapon*)i_EntList->GetClientEntityFromHandle((CBaseHandle)hActiveWeapon());
}
MoveType_t C_BaseEntity::GetMoveType() 
{
	return *reinterpret_cast<MoveType_t*>((uintptr_t)this + 0x258);
}
void C_BaseEntity::SetAngle2(Vector wantedang)
{
	typedef void(__thiscall* SetAngleFn)(void*, const Vector &);
	static SetAngleFn SetAngle2 = (SetAngleFn)((DWORD)g_Utils->FindPattern("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1", "xxxxxxxxxxxxxx"));
	SetAngle2(this, wantedang);
}

int C_BaseEntity::GetMoney()
{
	return *reinterpret_cast<int*>((uintptr_t)this + NetVars.m_iAccountID);
}

int C_BaseEntity::GetHitboxSet()
{
	return *reinterpret_cast<int*>((uintptr_t)this + NetVars.m_nHitboxSet);
}

int C_BaseEntity::GetPing()
{
	return *(int*)(NetVars.playerResource + NetVars.m_iPing + this->GetIndex() * 4);
	return *reinterpret_cast<int*>((uintptr_t)this + NetVars.m_iPing);
}