#include "Hooks.h"
#include "FakeLatency.h"
#include "VMT.h"
#include "ImMenu.h"
#include "Ragebot.h"

CreateMove oCreateMove;
DoPostScreenEffects oDoPostScreenEffects;
PaintTraverse oPaintTraverse;
OverrideView oOverrideView;
SceneEnd oSceneEnd;
iFindMdl oFindModel;
FrameStageNotify oFrameStageNotify;
WriteUsercmdDeltaToBuffer oWriteUsercmdDeltaToBuffer;
PlaySoundT oPlaySound;
EndScene oEndScene;
Reset oReset;
DrawModelExecute oDrawModelExecute;
SendDatagramFn oSendDatagram;
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



void __fastcall  Hooks::PaintTraverseHook(void *thisptr, void * _EDX,vgui::VPANEL panel, bool forceRepaint, bool allowForce) 
{
	static uint32_t FocusOverlayPanel;
	if (!FocusOverlayPanel)
	{
		const char* szName = i_Panel->GetName(panel);

		if (lstrcmpA(szName, "MatSystemTopPanel") == 0)
		{
			FocusOverlayPanel = panel;
			g_SDK->InitFonts();
			i_Engine->ExecuteClientCmd("clear");
			i_Cvar->ConsoleColorPrintf(Color(255, 0, 176, 255), "Project Serenity\n");
			i_Cvar->ConsoleColorPrintf(Color(255, 0, 176, 255), "Injected: %s %s \n  ", __TIME__, __DATE__);
			i_Engine->ExecuteClientCmd("toggleconsole");

		}
	}

	if (FocusOverlayPanel == panel)
	{
		auto local = i_EntList->GetClientEntity(i_Engine->GetLocalPlayer());
		time_t _tm = time(NULL);
		struct tm * curtime = localtime(&_tm);
		std::string time = asctime(curtime);
			g_Menu->Render();

		g_Render->text(150, 3, "Hi Laxai. Hi Pasters :P.", Font_Menu, Color(255, 255, 255, 255));
		g_Render->text(150, 18, time.c_str(), Font_Menu, Color(255, 0, 0, 255));

		/*if (local)
		{*/
			f_Esp->Start();
		//}
	}

	return oPaintTraverse(thisptr, panel, forceRepaint, allowForce);
}

float NormalizeYaw(float value)
{
	while (value > 180)
		value -= 360.f;

	while (value < -180)
		value += 360.f;
	return value;
}

void ClampAngles(Vector& angles)
{
	if (angles.x > 89.f)
		angles.x = 89.f;
	if (angles.x < -89.f)
		angles.x = -89.f;
	if (angles.y > 180.f)
		angles.y = 180.f;
	if (angles.y < -180.f)
		angles.y = -180.f;

	angles.z = 0.f;
}

void angleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	Math::SinCos(DEG2RAD(angles[1]), &sy, &cy);
	Math::SinCos(DEG2RAD(angles[0]), &sp, &cp);
	Math::SinCos(DEG2RAD(angles[2]), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}

void normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

static bool hooked = false;

bool __stdcall Hooks::CreateMoveHook(float flInputSampleTime, CUserCmd* cmd) 
{
	if (!cmd->command_number || !i_Engine->IsConnected() && !i_Engine->IsInGame()) 
		return true;

	g_Globals.cmd = cmd;

	C_BaseEntity* localplayer = i_EntList->GetClientEntity(i_Engine->GetLocalPlayer());

	if (!localplayer) 
		return true;

	g_Globals.localplayer = localplayer;

	PVOID pebp;
    __asm mov pebp, ebp;
    bool* bSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
    g_Globals.bSendPacket = *bSendPacket;

	if (!bSendPacket)
		return false;


	if (g_Vars.misc.FakeLatencyEnable && i_Engine->IsInGame() && i_Engine->IsConnected())
	{
		f_FakeLatency->UpdateIncomingSequences();
		auto clientState = *reinterpret_cast<uintptr_t*>(uintptr_t(GetModuleHandle("engine.dll")) + 0x586A74); //Do a patternscan and make a global var
		uintptr_t temp = *reinterpret_cast<uintptr_t*>(clientState + 0x9C); 
		INetChannel* netchan = reinterpret_cast<INetChannel*>(temp);//Do a global var

		if (!hooked && clientState) //ReInject after map change??? Detect mapchange and rehook
		{
			if (netchan)
			{
				hooked = true;
				CVMTHookManager* VMTNetChan = new CVMTHookManager((PDWORD*)netchan);
				oSendDatagram = (SendDatagramFn)(VMTNetChan->HookMethod((DWORD)SendDatagram, 46));
			}
		}
	}

	if(g_Vars.misc.Bhop) 
		f_Movement->Bhop(localplayer, cmd);

	if (g_Vars.misc.Bhop && g_Vars.misc.AutoStrafe)
		f_Movement->AutoStrafe(localplayer, cmd);

	Vector oldAngle = cmd->viewangles;
    float oldForward = cmd->forwardmove;
    float oldSideMove = cmd->sidemove;

	//f_Rage->Run(cmd);

	if(g_Vars.legit.AA)
		f_Misc->LegitAA(*bSendPacket);

	if(g_Vars.legit.Backtrack)
		f_LegitBacktrack.Run();

	f_Movement->MoveFix(oldAngle, oldForward, oldSideMove);

	return false;
}

int __fastcall Hooks::SendDatagram(INetChannel* netchan, void*, bf_write* datagram)
{
	if (datagram)
	{
		return oSendDatagram(netchan, datagram);
	}

	int instate = netchan->m_nInReliableState;
	int insequencenr = netchan->m_nInSequenceNr;

	float ammount = g_Vars.misc.FakeLatencyMode == 1 ? 0.2 : 0.8;

	if(g_Vars.misc.FakeLatencyEnable)
		f_FakeLatency->AddLatencyToNetchan(netchan, ammount);

	int ret = oSendDatagram(netchan, datagram);

	netchan->m_nInReliableState = instate;
	netchan->m_nInSequenceNr = insequencenr;
	return ret;
}

int  __fastcall Hooks::DoPostScreenEffectsHook(void *thisptr, void * _EDX,int a1) 
{
	if(i_Engine->GetLocalPlayer() && g_Vars.visuals.Glow) 
		f_Glow->Start();

	return oDoPostScreenEffects(thisptr,a1);
}

void __stdcall Hooks::OverrideViewHook(CViewSetup* pSetup)
{
	if (g_Vars.visuals.ViewModelFov != 0)
	{
		if(g_Vars.visuals.ViewModelFov == 1)
			if(pSetup->fov == 90)
				pSetup->fov = 100;

		if(g_Vars.visuals.ViewModelFov == 2)
			if(pSetup->fov == 90 || pSetup->fov == 100)
				pSetup->fov = 120;
	}

	oOverrideView(pSetup);
}

void __stdcall Hooks::FrameStageNotifyHook(ClientFrameStage_t stage)
{
	if(!i_Engine->IsConnected() || !i_Engine->IsInGame())
		return oFrameStageNotify(stage);

	C_BaseEntity* localplayer = g_Globals.localplayer;

	if (!localplayer)
		return;

	

	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) //Resolver? and skinchanger
	{
		//f_Resolver.Fsn();
		f_FakeLatency->UpdateIncomingSequences();
		//f_SkinChanger->Run();
	}

	if (stage == FRAME_RENDER_START) //Rage backtracking updates
	{
		
	}

	return oFrameStageNotify(stage);
}

void __fastcall Hooks::SceneEndHook(void* thisptr)
{
	oSceneEnd(thisptr);

	C_BaseEntity* localplayer = i_EntList->GetClientEntity(i_Engine->GetLocalPlayer());
	
	if (g_Vars.visuals.BacktrackChams != 0 && g_Vars.legit.Backtrack)
		f_Chams->BacktrackChams();

	if (g_Vars.visuals.ChamsStyle > 0)
		f_Chams->Start();
}

MDLHandle_t __fastcall Hooks::FindModelHook(void* ecx, void* edx, char* FilePath)
{
	return oFindModel(ecx, FilePath);
}

bool __fastcall Hooks::WriteUsercmdDeltaToBufferHook(IBaseClientDLL* this0, void * _EDX, int nSlot, void* buf, int from, int to, bool isNewCmd) 
{
	return true;
}

void __stdcall Hooks::PlaySoundHook(const char *folderIme) 
{
	return oPlaySound(i_Surface, folderIme);
}

void __stdcall Hooks::DrawModelExecuteHook(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld) 
{
	if (!i_Engine->IsConnected() && !i_Engine->IsInGame())
		return;

	static IMaterial* mat1 = i_MatSystem->FindMaterial("reflective_normal", TEXTURE_GROUP_MODEL);
	mat1->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);

	static IMaterial* mat2 = i_MatSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_MODEL);

	const char* name = pInfo.pModel->name;

	if (g_Vars.visuals.WeaponChams > 0 && (strstr(name, "arms") || strstr(name, "models/weapons")))
	{
		i_ModelRender->ForcedMaterialOverride(mat1);
		i_RenderView->SetColorModulation(1, 0, 1);
		oDrawModelExecute(i_ModelRender,ctx, state, pInfo, pCustomBoneToWorld);
		i_ModelRender->ForcedMaterialOverride(nullptr);
		i_RenderView->SetColorModulation(0, 0, 0);
	}

	return oDrawModelExecute(i_ModelRender,ctx, state, pInfo, pCustomBoneToWorld);
}

HRESULT __stdcall Hooks::EndSceneHook(IDirect3DDevice9 *pDevice)
{

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);

	if (!ImMenu::D3Init)
		ImMenu::GuiInit(m_hwndWindow, pDevice);

	ImGui::GetIO().MouseDrawCursor = g_Vars.menu.OpenedMenu;

	ImGui_ImplDX9_NewFrame();

	if (g_Vars.menu.OpenedMenu)
		ImMenu::Draw();

	ImGui::Render();


	return oEndScene(pDevice);
}

HRESULT __stdcall Hooks::ResetHook(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	if (!ImMenu::D3Init)
		return oReset(pDevice, pPresentationParameters);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = oReset(pDevice, pPresentationParameters);

	if (hr >= 0)
		ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}

LRESULT __stdcall Hooks::WndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:

		pressedKey[VK_LBUTTON] = true;
		break;

	case WM_LBUTTONUP:

		pressedKey[VK_LBUTTON] = false;
		break;

	case WM_RBUTTONDOWN:

		pressedKey[VK_RBUTTON] = true;
		break;

	case WM_RBUTTONUP:

		pressedKey[VK_RBUTTON] = false;
		break;

	case WM_KEYDOWN:

		pressedKey[wParam] = true;
		break;

	case WM_KEYUP:

		pressedKey[wParam] = false;
		break;

	default: break;
	}
	ImMenu::Open();

	if (ImMenu::D3Init && g_Vars.menu.OpenedMenu && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oldWindowProc, hWnd, uMsg, wParam, lParam);
}