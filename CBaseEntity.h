#pragma once
#include "SDK.h"

class AnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038

class C_BaseEntity 
{
public:
	bool IsValid();
	bool IsTeam();

	bool IsPlayer();
	bool IsWeapon();
	ICollideable* GetCollideable();
	const model_t* GetModel();
	bool SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	ClientClass* GetClientClass();
	int GetIndex();
	int GetFlags();
	Vector GetViewOffset();
	Vector GetEyePos();
	int GetHealth();
	int GetTeam();
	bool GetDormant();
	Vector GetOrigin();
	void SetOrigin(Vector origin);
	void SetAngles(const QAngle &angles);
	QAngle& GetAngles();
	Vector GetPunchAngles();
	QAngle QAngPunch();
	CBaseHandle hActiveWeapon();
	C_BaseWeapon* GetActiveWeapon();
	const matrix3x4_t& GetCoordinateFrame();
	Vector GetHitboxPos(int hitbox_id);
	bool GetHitboxPos(int hitbox, Vector &output);
	Vector GetBonePos(int bone);
	bool CanSeePlayer(C_BaseEntity* player, int hitbox);
	bool CanSeePlayer(C_BaseEntity* player, const Vector& pos);
	bool bSpotted();
	int GetArmor();
	int DrawModel(int flags, uint8_t alpha);
	void* GetClientRenderable();
	int GetTickBase();
	int GetMoney();
	float GetSimTime();
	std::array<float, 24> &m_flPoseParameter();
	float NextAttack();
	bool HasHelmet();
	QAngle* EyeAngs();
	float Lby();
	AnimationLayer& GetAnimOverlay(int Index);
	AnimationLayer* GetAnimOverlays();
	int GetSequence();
	int GetSequenceActivity(int sequence);
	float GetAnimationTime();
	matrix3x4_t GetBoneMatrix(int BoneID);
	Vector GetVelocity();
	MoveType_t GetMoveType();
	void SetAngle2(Vector wantedang);
	int GetNumAnimOverlays();
	float GetCycle();
	int GetHitboxSet();
	int GetPing();
};

/* 10 */
enum CSGO_ACTS
{
	ACT_CSGO_DEFUSE = 958,
	ACT_CSGO_DEFUSE_WITH_KIT = 959,
	ACT_CSGO_FLASHBANG_REACTION = 960,
	ACT_CSGO_FIRE_PRIMARY = 961, // when shooting
	ACT_CSGO_FIRE_PRIMARY_OPT_1 = 962,
	ACT_CSGO_FIRE_PRIMARY_OPT_2 = 963,
	ACT_CSGO_FIRE_SECONDARY = 964, // shooting secondary
	ACT_CSGO_RELOAD = 967,
	ACT_CSGO_RELOAD_START = 968,
	ACT_CSGO_RELOAD_LOOP = 969,
	ACT_CSGO_RELOAD_END = 970,
	ACT_CSGO_PLANT_BOMB = 978,
	ACT_CSGO_IDLE_TURN_BALANCEADJUST = 979,
	ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING = 980,
};