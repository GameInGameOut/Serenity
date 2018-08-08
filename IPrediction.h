#pragma once

#include "SDK.h"

class CMoveData
{
public:
	bool    m_bFirstRunOfFunctions : 1;
	bool    m_bGameCodeMovedPlayer : 1;
	int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
	int     m_nImpulseCommand;      // Impulse command issued.
	Vector  m_vecViewAngles;        // Command view angles (local space)
	Vector  m_vecAbsViewAngles;     // Command view angles (world space)
	int     m_nButtons;             // Attack buttons.
	int     m_nOldButtons;          // From host_client->oldbuttons;
	float   m_flForwardMove;
	float   m_flSideMove;
	float   m_flUpMove;
	float   m_flMaxSpeed;
	float   m_flClientMaxSpeed;
	Vector  m_vecVelocity;          // edict::velocity        // Current movement direction.
	Vector  m_vecAngles;            // edict::angles
	Vector  m_vecOldAngles;
	float   m_outStepHeight;        // how much you climbed this move
	Vector  m_outWishVel;           // This is where you tried 
	Vector  m_outJumpVel;           // This is your jump velocity
	Vector  m_vecConstraintCenter;
	float   m_flConstraintRadius;
	float   m_flConstraintWidth;
	float   m_flConstraintSpeedFactor;
	float   m_flUnknown[5];
	Vector  m_vecAbsOrigin;
};

class C_BaseEntity;

class IGameMovement
{
public:
	virtual			~IGameMovement(void) {}

	virtual void	          ProcessMovement(C_BaseEntity *pPlayer, CMoveData *pMove) = 0;
	virtual void	          Reset(void) = 0;
	virtual void	          StartTrackPredictionErrors(C_BaseEntity *pPlayer) = 0;
	virtual void	          FinishTrackPredictionErrors(C_BaseEntity *pPlayer) = 0;
	virtual void	          DiffPrint(char const *fmt, ...) = 0;
	virtual Vector const&	  GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	  GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;
	virtual bool		        IsMovingPlayerStuck(void) const = 0;
	virtual C_BaseEntity*   GetMovingPlayer(void) const = 0;
	virtual void		        UnblockPusher(C_BaseEntity *pPlayer, C_BaseEntity *pPusher) = 0;
	virtual void            SetupMovementBounds(CMoveData *pMove) = 0;
};

class CGameMovement
	: public IGameMovement
{
public:
	virtual ~CGameMovement(void) {}
}; extern CGameMovement* i_GameMovement;

class IPrediction
{
public:
	bool InPrediction()
	{
		typedef bool(__thiscall* oInPrediction)(void*);
		return VT::VFunc<oInPrediction>(this, 14)(this);
	}

	void RunCommand(C_BaseEntity *player, CUserCmd *ucmd, IMoveHelper *moveHelper)
	{
		typedef void(__thiscall* oRunCommand)(void*, C_BaseEntity*, CUserCmd*, IMoveHelper*);
		return VT::VFunc<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
	}

	void SetupMove(C_BaseEntity *player, CUserCmd *ucmd, IMoveHelper *moveHelper, void* pMoveData)
	{
		typedef void(__thiscall* oSetupMove)(void*, C_BaseEntity*, CUserCmd*, IMoveHelper*, void*);
		return VT::VFunc<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void FinishMove(C_BaseEntity *player, CUserCmd *ucmd, void*pMoveData)
	{
		typedef void(__thiscall* oFinishMove)(void*, C_BaseEntity*, CUserCmd*, void*);
		return VT::VFunc<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
	}
}; extern IPrediction* i_Prediction;