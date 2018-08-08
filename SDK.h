#pragma once

//Serenity SDK
//Qotq but a lil bit better 

#include <windows.h>
#include <SDKDDKVer.h>
#include <string.h>
#include <vadefs.h>
#include <stdio.h>
#include <xstring>
#include <Psapi.h>
#include <thread>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>
#include <time.h>
#include <WinUser.h>
#include <random>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <set>
#include <map>
#include <unordered_map>
#include <memory>
#include <thread>
#include <array>
#include <assert.h>
#include <deque>
#include <intrin.h>
#include <atomic>
#include <mutex>
#include <d3d9types.h>
#include <d3d9.h>
//some defines
#define INRANGE(x, a, b) (x >= a && x <= b) 
#define getBits(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (INRANGE(x, '0', '9') ? x - '0': 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

//sdk includes
#include "Utils.h"
#include "VFunc.h"
#include "Hooking.h"


//forward declaration
class C_BaseEntity;
class C_BaseWeapon;

#include "Imgui\imgui.h"
#include "Imgui\imgui_internal.h"
#include "Imgui\DX9\imgui_impl_dx9.h"
#include "Imgui\imconfig.h"
#include "Imgui\stb_rect_pack.h"
#include "datamap.h"
#include "characterset.h"
#include "checksum_crc.h"
#include "checksum_md5.h"
#include "Enums.h"
#include "Recv.h"
#include "NetvarManager.h"
#include "BaseHandle.h"
#include "ClientClass.h"
#include "Color.h"
#include "CUserCmd.h"
#include "CViewSetup.h"
#include "GlobalVars.h"
#include "IAppSystem.h"
#include "IHandleEntity.h"
#include "QAngle.h"
#include "Vector.h"
#include "Vector2D.h"
#include "Vector4D.h"
#include "VMatrix.h"
#include "Math.h"
#include "INetChannelInfo.h"
#include "IConVar.h"
#include "GlowObjectManager.h"
#include "UtlBuffer.h"
#include "UtlMemory.h"
#include "UtlString.h"
#include "UtlVector.h"
#include "Convar.h"
#include "Studio.h"
#include "ICollideable.h"
#include "IClientNetworkable.h"
#include "IClientRenderable.h"
#include "IClientThinkable.h"
#include "IClientUnknown.h"
#include "IClientEntity.h"
#include "CBaseEntity.h"
#include "CBaseWeapon.h"
#include "LegitBacktrack.h"

#include "Variables.h"
#include "IPanel.h"
#include "IClientMode.h"
#include "IBaseClientDLL.h"
#include "ISurface.h"
#include "IVEngineClient.h"
#include "ICvar.h"
#include "IClientEntityList.h"
#include "IEngineTrace.h"
#include "IVDebugOverlay.h"
#include "CInput.h"
#include "IVModelInfoClient.h"
#include "IVModelRender.h"
#include "IRenderView.h"
#include "IPhysicsProps.h"
#include "IMaterialSystem.h"
#include "IGameEventManager.h"
#include "IMoveHelper.h"
#include "IMDLCache.h"
#include "IPrediction.h"
#include "CClientState.h"
#include "IInputSystem.h"

extern vgui::HFont Font_Menu;
extern vgui::HFont Font_ESP;
extern WNDPROC oldWindowProc;
extern HWND m_hwndWindow;
extern bool pressedKey[256];
#include "DrawManager.h"
#include "Menu.h"
//hooks 
#include "Hooks.h"

//features includes
#include "Glow.h"
#include "ESP.h"
#include "Movement.h"
#include "Chams.h"
#include "Misc.h"

//some sdk things 
using namespace std;
//using namespace VT;
//more sdk things
class CSDK 
{
public:
	void InitFonts();
}; extern CSDK* g_SDK;

class CGlobals
{
public:
	bool bSendPacket;
	CUserCmd* cmd;
	C_BaseEntity* localplayer;
}; extern CGlobals g_Globals;


class Offsets
{
public:
	DWORD playerResource;
	DWORD m_iPing = 0;
	DWORD m_flCycle = 0;
	DWORD m_rgflCoordinateFrame = 0;
	DWORD m_Collision = 0;
	DWORD m_vecOrigin = 0;
	DWORD m_vecViewOffset = 0;
	DWORD m_vecPunchAngles = 0;
	DWORD m_vecViewPunchAngles = 0;
	DWORD m_vecVelocity = 0;
	DWORD m_bIsScoped = 0;
	DWORD m_lifeState = 0;
	DWORD m_fFlags = 0;
	DWORD m_iHealth = 0;
	DWORD m_iTeamNum = 0;
	DWORD m_iGlowIndex = 0;
	DWORD m_flFlashDuration;
	DWORD m_iShotsFired = 0;
	DWORD m_flFlashMaxAlpha = 0;
	DWORD m_hActiveWeapon = 0;
	DWORD m_hMyWeapons = 0;
	DWORD m_ArmorValue = 0;
	DWORD m_nHitboxSet = 0;
	DWORD m_nModelIndex = 0;
	DWORD m_hOwner = 0;
	DWORD m_hWeapon = 0;
	DWORD m_hViewModel = 0;
	DWORD m_nTickBase = 0;
	DWORD m_hObserverTarget = 0;
	DWORD m_flSimulationTime = 0;
	DWORD m_flLowerBodyYawTarget = 0;
	DWORD m_flC4Blow = 0;
	DWORD m_flTimerLength = 0;
	DWORD m_hMyWearables = 0;
	DWORD m_angEyeAngles = 0;
	DWORD m_nSequence = 0;
	DWORD m_flPoseParameter = 0;

	DWORD m_iAccountID = 0;
	DWORD m_iItemDefinitionIndex = 0;
	DWORD m_iItemIDHigh = 0;
	DWORD m_iEntityQuality = 0;
	DWORD m_szCustomName = 0;
	DWORD m_nFallbackPaintKit = 0;
	DWORD m_flFallbackWear = 0;
	DWORD m_nFallbackSeed = 0;
	DWORD m_nFallbackStatTrak = 0;
	DWORD m_OriginalOwnerXuidLow = 0;
	DWORD m_OriginalOwnerXuidHigh = 0;

	//Weapons
	DWORD m_flNextPrimaryAttack = 0;
	DWORD m_flNextSecondaryAttack = 0;
	DWORD m_iClip1 = 0;
	DWORD m_iClip2 = 0;
}; extern Offsets NetVars;
class CNetVars;
class COffsets
{
public:
	std::shared_ptr<CNetVars> NetVars = nullptr;
}; extern COffsets g_Offsets;


