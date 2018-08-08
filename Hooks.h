#pragma once
#include "SDK.h"
//typedefs
using CreateMove = bool(__stdcall*)(float, CUserCmd*);
extern CreateMove oCreateMove;

using DoPostScreenEffects = int(__thiscall*)(void*,int);
extern DoPostScreenEffects oDoPostScreenEffects;

using PaintTraverse = void(__thiscall*)(void*,vgui::VPANEL, bool, bool);
extern PaintTraverse oPaintTraverse;

using OverrideView = int(__stdcall*)(CViewSetup*);
extern OverrideView oOverrideView;

using SceneEnd = void(__fastcall*)(void*);
extern SceneEnd oSceneEnd;

typedef MDLHandle_t(__thiscall *iFindMdl)(void*, char*);
extern iFindMdl oFindModel;

using FrameStageNotify = void(__stdcall*)(ClientFrameStage_t);
extern FrameStageNotify oFrameStageNotify;

using WriteUsercmdDeltaToBuffer = void(__thiscall*)(IBaseClientDLL*, void * , int , void* , int , int , bool );
extern WriteUsercmdDeltaToBuffer oWriteUsercmdDeltaToBuffer;

using PlaySoundT = void(__thiscall*)(ISurface*, const char*);
extern PlaySoundT oPlaySound;


using EndScene = long(__stdcall*)(IDirect3DDevice9 *);
extern EndScene oEndScene;

using Reset = long(__stdcall*)(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS*);
extern Reset oReset;

using DrawModelExecute = void(__thiscall*)(IVModelRender* ,IMatRenderContext*, const DrawModelState_t& , const ModelRenderInfo_t& , matrix3x4_t*);
extern DrawModelExecute oDrawModelExecute;

using SendDatagramFn = int(__thiscall*)(INetChannel*, bf_write *);
extern SendDatagramFn oSendDatagram;

//Hooking shit

namespace Hooks {
	extern bool __stdcall CreateMoveHook(float flInputSampleTime, CUserCmd* cmd);
	extern int  __fastcall DoPostScreenEffectsHook(void *thisptr, void * _EDX,int a1);
	extern void __fastcall PaintTraverseHook(void *thisptr, void * _EDX,vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	extern void __stdcall OverrideViewHook(CViewSetup* pSetup);
	extern void __stdcall FrameStageNotifyHook(ClientFrameStage_t stage);
	extern void __fastcall SceneEndHook(void* thisptr);
	extern bool __fastcall WriteUsercmdDeltaToBufferHook(IBaseClientDLL* this0, void * _EDX, int nSlot, void* buf, int from, int to, bool isNewCmd);
	extern void __stdcall PlaySoundHook(const char *folderIme);
	extern LRESULT __stdcall WndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern HRESULT __stdcall EndSceneHook(IDirect3DDevice9 *pDevice);
	extern HRESULT __stdcall ResetHook(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);
	extern void __stdcall DrawModelExecuteHook(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	extern int __fastcall SendDatagram(INetChannel* netchan, void*, bf_write* datagram);
	extern MDLHandle_t __fastcall FindModelHook(void* ecx, void* edx, char* FilePath);
}
