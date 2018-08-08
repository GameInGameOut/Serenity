#include "SDK.h"
#include "VMT.h"

HWND m_hwndWindow = NULL;
WNDPROC oldWindowProc = nullptr;
bool pressedKey[256] = {};

CSDK* g_SDK;
Variables g_Vars;
Menu* g_Menu;
CUtils* g_Utils;
Renderer* g_Render;
CGlobals g_Globals;

CGlow* f_Glow;
CGlowObjectManager* f_GlowManager;
CESP* f_Esp;
CMovement* f_Movement;
CChams* f_Chams;

vgui::HFont Font_Menu;
vgui::HFont Font_ESP;
Offsets NetVars;
COffsets g_Offsets;


IBaseClientDLL* i_Client = nullptr;
IClientMode* i_ClientMode = nullptr;
IPanel* i_Panel = nullptr;
ISurface* i_Surface = nullptr;
IVEngineClient* i_Engine = nullptr;
ICvar* i_Cvar = nullptr;
IClientEntityList* i_EntList = nullptr;
CGlobalVarsBase* i_GlobalVars = nullptr;
IEngineTrace* i_Trace = nullptr;
IVDebugOverlay* g_pDebugOverlay = nullptr;
CInput* i_Input = nullptr;
IVModelInfoClient* i_ModelInfo = nullptr;
IVModelRender* i_ModelRender = nullptr;
IVRenderView* i_RenderView = nullptr;
IMaterialSystem* i_MatSystem = nullptr;
IGameEventManager2* i_Events = nullptr;
IMoveHelper* i_MoveHelper = nullptr;
IMDLCache* g_pMDLCache = nullptr;
IPrediction* i_Prediction = nullptr;
CGameMovement* i_GameMovement = nullptr;
CClientState* i_ClientState = nullptr;
IInputSystem* i_InputSystem = nullptr;
IPhysicsSurfaceProps* i_SurfaceProps = nullptr;

surfacedata_t* IPhysicsSurfaceProps::GetSurfaceData(int surfaceDataIndex)
{
	typedef surfacedata_t*(__thiscall* fnGetSurfaceData)(void*, int);
	return VT::VFunc<fnGetSurfaceData>(this, 5)(this, surfaceDataIndex);
}

template< typename T >
T* GetInterface(char* mod, char* interf, bool exact = false)
{
	typedef T* (*CreateInterfaceFn)(const char* szName, int iReturn);
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(mod), ("CreateInterface"));

	std::string sInterface, sInterfaceVersion = "0";

	if (!exact)
	{
		for (auto i = 0; i <= 99; i++)
		{
			sInterface = interf;
			sInterface += sInterfaceVersion;
			sInterface += std::to_string(i);

			T* funcPtr = CreateInterface(sInterface.c_str(), NULL);

			if (funcPtr != NULL)
			{
				g_Utils->Print("%s -> 0x%x", sInterface.c_str(), funcPtr);
				return funcPtr;
			}

			if (i == 99 && sInterfaceVersion == "0")
			{
				sInterfaceVersion = "00";
				i = 0;
			}
			else if (i == 99 && sInterfaceVersion == "00")
			{
				g_Utils->Print("%s -> Cant get pointer!", interf);
				return nullptr;
			}
		}
	}
	else
	{
		T* ptr = CreateInterface(interf, 0);

		if (ptr)
		{
			g_Utils->Print("%s -> 0x%x", interf, ptr);
			return ptr;
		}
		else
		{
			g_Utils->Print("%s ->  Cant get pointer!", interf);
			return nullptr;
		}
	}

	return nullptr;
}

void InitialThread(void)
{
	Beep(1000, 100);
	while (!(m_hwndWindow = FindWindowA("Valve001", NULL)))
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	while (!GetModuleHandleA("serverbrowser.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		std::ofstream("csgo\\materials\\reflective_ignorez.vmt") << R"#("VertexLitGeneric"
	{

	  "$basetexture" "vgui/white_additive"
	  "$ignorez"      "1"
	  "$envmap"       "env_cubemap"
	  "$normalmapalphaenvmapmask"  "1"
	  "$envmapcontrast"             "1"
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	std::ofstream("csgo\\materials\\reflective_normal.vmt") << R"#("VertexLitGeneric"
	{

	  "$basetexture" "vgui/white_additive"
	  "$ignorez"      "0"
	  "$envmap"       "env_cubemap"
	  "$normalmapalphaenvmapmask"  "1"
	  "$envmapcontrast"             "1"
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	std::ofstream("csgo\\materials\\textured_ignorez.vmt") << R"#("VertexLitGeneric"
	{
	  "$basetexture" "vgui/white_additive"
	  "$ignorez"      "1"
	  "$envmap"       ""
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$selfillum"    "1"
	  "$halflambert"  "1"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	std::ofstream("csgo\\materials\\textured_normal.vmt") << R"#("VertexLitGeneric"
	{
	  "$basetexture" "vgui/white_additive"
	  "$ignorez"      "0"
	  "$envmap"       ""
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$selfillum"    "1"
	  "$halflambert"  "1"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	std::ofstream("csgo\\materials\\flat_normal.vmt") << R"#("UnlitGeneric"
	{
	  "$basetexture" "vgui/white_additive"
	  "$ignorez"      "0"
	  "$envmap"       ""
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$selfillum"    "1"
	  "$halflambert"  "1"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	std::ofstream("csgo\\materials\\flat_ignorez.vmt") << R"#("UnlitGeneric"
	{
	  "$basetexture" "vgui/white_additive"
	  "$ignorez"      "1"
	  "$envmap"       ""
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$selfillum"    "1"
	  "$halflambert"  "1"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	

	g_Utils->Print("-------------------------------------------------------------------------------");
	g_Utils->Print("");
	g_Utils->Print("-------------------------------------------------------------------------------");
	g_Utils->Print("----------------------------Project Serenity-----------------------------------");
	g_Utils->Print("-------------------------------------------------------------------------------");
	g_Utils->Print("");
	g_Utils->Print("-------------------------------------------------------------------------------");
	//-----------------------------------------------------------------------------
	// Getting interfaces.
	//-----------------------------------------------------------------------------
	g_Utils->Print("Getting Interfaces:");

	i_Client = GetInterface<IBaseClientDLL>("client_panorama.dll", "VClient");

	i_ClientMode = **reinterpret_cast<IClientMode***>((*(uintptr_t**)i_Client)[10] + 0x5);
	g_Utils->Print("ClientMode -> 0x%x", i_ClientMode);

	i_Panel = GetInterface<IPanel>("vgui2.dll", "VGUI_Panel");

	i_Surface = GetInterface<ISurface>("vguimatsurface.dll", "VGUI_Surface");

	i_Engine = GetInterface<IVEngineClient>("engine.dll", "VEngineClient");

	i_Cvar = GetInterface<ICvar>("vstdlib.dll", "VEngineCvar");

	f_GlowManager = *(CGlowObjectManager**)(g_Utils->FindPatternIDA("client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);
	g_Utils->Print("GlowObjectManager -> 0x%x", f_GlowManager);

	i_EntList = GetInterface<IClientEntityList>("client_panorama.dll", "VClientEntityList");

	i_GlobalVars = **(CGlobalVarsBase***)((*(DWORD**)(i_Client))[0] + 0x1B);
	g_Utils->Print("GlobalVarsBase -> 0x%x", i_GlobalVars);

	i_Trace = GetInterface<IEngineTrace>("engine.dll", "EngineTraceClient");

	g_pDebugOverlay = GetInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay");;

	i_Input = *(CInput**)((*(DWORD**)i_Client)[15] + 0x1);;
	g_Utils->Print("Input -> 0x%x", i_Input);

	i_ModelRender = GetInterface<IVModelRender>("engine.dll", "VEngineModel");

	i_ModelInfo = GetInterface<IVModelInfoClient>("engine.dll", "VModelInfoClient");

	i_RenderView = GetInterface<IVRenderView>("engine.dll", "VEngineRenderView");

	i_MatSystem = GetInterface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");

	i_Events = GetInterface<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002", true);

	i_MoveHelper = **(IMoveHelper***)(g_Utils->FindPatternIDA("client_panorama.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
	g_Utils->Print("IMoveHelper -> 0x%x", i_MoveHelper);

	g_pMDLCache = GetInterface<IMDLCache>("datacache.dll", "MDLCache");

	i_Prediction = GetInterface<IPrediction>("client_panorama.dll", "VClientPrediction");

	i_GameMovement = GetInterface<CGameMovement>("client_panorama.dll", "GameMovement");

	i_ClientState = **(CClientState***)((*(DWORD**)i_Engine)[12] + 0x10);
	g_Utils->Print("CClientState -> 0x%x", i_ClientState);

	i_InputSystem = GetInterface<IInputSystem>("inputsystem.dll", "InputSystemVersion");

	auto D3DDevice9 = **(IDirect3DDevice9***)(g_Utils->FindPatternIDA("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	g_Utils->Print("IDirect3DDevice9 -> 0x%x", D3DDevice9);

	i_SurfaceProps = GetInterface<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps");
	g_Utils->Print("IPhysicsSurfaceProps -> 0x%x", i_SurfaceProps);
		
	g_Utils->Print("-------------------------------------------------------------------------------");
	g_Utils->Print("Dumping Netvars:");
	g_Utils->Print("-------------------------------------------------------------------------------");


	//-----------------------------------------------------------------------------
	// Netvars.
	//-----------------------------------------------------------------------------

	//pasted from my old paste which i pasted from somewhere, dont remember.
	g_Offsets.NetVars = std::shared_ptr<CNetVars>(new CNetVars());
	g_Offsets.NetVars->GetOffset(("DT_BaseEntity"), ("m_CollisionGroup"), &NetVars.m_rgflCoordinateFrame, -0x30);
	g_Offsets.NetVars->GetOffset(("DT_BaseEntity"), ("m_Collision"), &NetVars.m_Collision);
	g_Offsets.NetVars->GetOffset(("DT_BaseEntity"), ("m_vecOrigin"), &NetVars.m_vecOrigin);
	g_Offsets.NetVars->GetOffset(("DT_BaseEntity"), ("m_iTeamNum"), &NetVars.m_iTeamNum);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_aimPunchAngle"), &NetVars.m_vecPunchAngles);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_viewPunchAngle"), &NetVars.m_vecViewPunchAngles);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_vecViewOffset[0]"), &NetVars.m_vecViewOffset);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_ArmorValue"), &NetVars.m_ArmorValue);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_vecVelocity[0]"), &NetVars.m_vecVelocity);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_lifeState"), &NetVars.m_lifeState);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_fFlags"), &NetVars.m_fFlags);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_iHealth"), &NetVars.m_iHealth);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_hActiveWeapon"), &NetVars.m_hActiveWeapon);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_hMyWeapons"), &NetVars.m_hMyWeapons);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_hViewModel[0]"), &NetVars.m_hViewModel);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_nTickBase"), &NetVars.m_nTickBase);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_hObserverTarget"), &NetVars.m_hObserverTarget);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_flSimulationTime"), &NetVars.m_flSimulationTime);
	g_Offsets.NetVars->GetOffset(("DT_BasePlayer"), ("m_nHitboxSet"), &NetVars.m_nHitboxSet);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_iAccount"), &NetVars.m_iAccountID);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_iItemDefinitionIndex"), &NetVars.m_iItemDefinitionIndex);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_iItemIDHigh"), &NetVars.m_iItemIDHigh);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_iEntityQuality"), &NetVars.m_iEntityQuality);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_szCustomName"), &NetVars.m_szCustomName);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_nFallbackPaintKit"), &NetVars.m_nFallbackPaintKit);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_flFallbackWear"), &NetVars.m_flFallbackWear);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_nFallbackSeed"), &NetVars.m_nFallbackSeed);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_nFallbackStatTrak"), &NetVars.m_nFallbackStatTrak);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_OriginalOwnerXuidLow"), &NetVars.m_OriginalOwnerXuidLow);
	g_Offsets.NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_OriginalOwnerXuidHigh"), &NetVars.m_OriginalOwnerXuidHigh);
	g_Offsets.NetVars->GetOffset(("DT_BaseViewModel"), ("m_nModelIndex"), &NetVars.m_nModelIndex);
	g_Offsets.NetVars->GetOffset(("DT_BaseViewModel"), ("m_hOwner"), &NetVars.m_hOwner);
	g_Offsets.NetVars->GetOffset(("DT_BaseViewModel"), ("m_hWeapon"), &NetVars.m_hWeapon);
	g_Offsets.NetVars->GetOffset(("DT_BaseCombatWeapon"), ("m_flNextPrimaryAttack"), &NetVars.m_flNextPrimaryAttack);
	g_Offsets.NetVars->GetOffset(("DT_BaseCombatWeapon"), ("m_flNextSecondaryAttack"), &NetVars.m_flNextSecondaryAttack);
	g_Offsets.NetVars->GetOffset(("DT_BaseCombatWeapon"), ("m_iClip1"), &NetVars.m_iClip1);
	g_Offsets.NetVars->GetOffset(("DT_BaseCombatWeapon"), ("m_iPrimaryReserveAmmoCount"), &NetVars.m_iClip2);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_bIsScoped"), &NetVars.m_bIsScoped);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_iShotsFired"), &NetVars.m_iShotsFired);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_flFlashMaxAlpha"), &NetVars.m_flFlashMaxAlpha);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_hMyWearables"), &NetVars.m_hMyWearables);
	g_Offsets.NetVars->GetOffset(("DT_PlantedC4"), ("m_flC4Blow"), &NetVars.m_flC4Blow);
	g_Offsets.NetVars->GetOffset(("DT_PlantedC4"), ("m_flTimerLength"), &NetVars.m_flTimerLength);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_flLowerBodyYawTarget"), &NetVars.m_flLowerBodyYawTarget);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_angEyeAngles"), &NetVars.m_angEyeAngles);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_flCycle"), &NetVars.m_flCycle);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_nSequence"), &NetVars.m_nSequence);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayer"), ("m_flPoseParameter"), &NetVars.m_flPoseParameter);
	g_Offsets.NetVars->GetOffset(("DT_CSPlayerResource"), ("m_iPing"), &NetVars.m_iPing);

	NetVars.playerResource = *(DWORD*)(g_Utils->FindPatternIDA("client_panorama.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 2);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	//-----------------------------------------------------------------------------
	// Hooking.
	//-----------------------------------------------------------------------------

	oldWindowProc = (WNDPROC)SetWindowLongPtr(m_hwndWindow, GWLP_WNDPROC, (LONG_PTR)Hooks::WndProcHook);

	//OLD HOOKING -- DETECTED

	g_Utils->Print("Hooking:");

	CVMTHookManager* VGuiHook = new CVMTHookManager((PDWORD*)i_Panel);
	oPaintTraverse = (PaintTraverse)(VGuiHook->HookMethod((DWORD)Hooks::PaintTraverseHook, 41));
	g_Utils->Print("PaintTraverse(41) -> 0x%x", oPaintTraverse);

	CVMTHookManager* ClientModeHook = new CVMTHookManager((PDWORD*)i_ClientMode);
	oCreateMove = (CreateMove)(ClientModeHook->HookMethod((DWORD)Hooks::CreateMoveHook, 24));
	g_Utils->Print("CreateMove(24) -> 0x%x", oCreateMove);

	oDoPostScreenEffects = (DoPostScreenEffects)(ClientModeHook->HookMethod((DWORD)Hooks::DoPostScreenEffectsHook, 44));
	g_Utils->Print("DoPostScreenEffects(44) -> 0x%x", oDoPostScreenEffects);

	oOverrideView = (OverrideView)(ClientModeHook->HookMethod((DWORD)Hooks::OverrideViewHook, 18));
	g_Utils->Print("OverrideView(18) -> 0x%x", oOverrideView);

	CVMTHookManager* ClientHook = new CVMTHookManager((PDWORD*)i_Client);
	oFrameStageNotify = (FrameStageNotify)(ClientHook->HookMethod((DWORD)Hooks::FrameStageNotifyHook, 37));
	g_Utils->Print("FrameStageNotify(36) -> 0x%x", oFrameStageNotify);

	CVMTHookManager* SurfaceHook = new CVMTHookManager((PDWORD*)i_Surface);
	oPlaySound = (PlaySoundT)(SurfaceHook->HookMethod((DWORD)Hooks::PlaySoundHook, 82));
	g_Utils->Print("PlaySound(82) -> 0x%x", oPlaySound);

	CVMTHookManager* RenderViewHook = new CVMTHookManager((PDWORD*)i_RenderView);
	oSceneEnd = (SceneEnd)(RenderViewHook->HookMethod((DWORD)Hooks::SceneEndHook, 9));
	g_Utils->Print("SceneEnd(9) -> 0x%x", oSceneEnd);

	CVMTHookManager* FindMdlHook = new CVMTHookManager((PDWORD*)g_pMDLCache);
	oFindModel = (iFindMdl)(FindMdlHook->HookMethod((DWORD)Hooks::FindModelHook, 10));

	/*CVMTHookManager* D3DDevHook = new CVMTHookManager((PDWORD*)D3DDevice9);
	oEndScene = (EndScene)(D3DDevHook->HookMethod((DWORD)Hooks::EndSceneHook, 42));
	g_Utils->Print("EndScene(42) -> 0x%x", oEndScene);

	oReset = (Reset)(D3DDevHook->HookMethod((DWORD)Hooks::ResetHook, 16));
	g_Utils->Print("Reset(16) -> 0x%x", oReset);*/

	CVMTHookManager* MdlRenderHook = new CVMTHookManager((PDWORD*)i_ModelRender);
	oDrawModelExecute = (DrawModelExecute)(MdlRenderHook->HookMethod((DWORD)Hooks::DrawModelExecuteHook, 21));
	g_Utils->Print("DrawModelExecute(21) -> 0x%x", oDrawModelExecute);

	Beep(1500, 100);
	g_Utils->Print("\n\n\n\n\n\n");
}



void CSDK::InitFonts() {
	Font_Menu = i_Surface->CreateFont_();
	Font_ESP = i_Surface->CreateFont_();

	i_Surface->SetFontGlyphSet(Font_Menu, "Verdana", 16, 600, 0, 0, FONTFLAG_OUTLINE);
	i_Surface->SetFontGlyphSet(Font_ESP, "Cornerstone", 10, 450, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
}