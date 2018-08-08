#include "Hooking.h"
#include "SDK.h"

bool Hooking::applyHook(void* hook)
{
	return setHookAtAddress(Hooking::funcPtr, hook);
}

bool Hooking::findFunc(char* libName, char* funcName)
{
	Hooking::funcPtr = (void*)GetProcAddress(GetModuleHandleA(libName), funcName);
	return (Hooking::funcPtr != NULL);
}

bool Hooking::reapplyHook()
{
	DWORD dwProtect;
	if (VirtualProtect(funcPtr, 6, PAGE_EXECUTE_READWRITE, &dwProtect))
	{
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)funcPtr, Hooking::hookData, 6, 0);
		VirtualProtect(funcPtr, 6, dwProtect, NULL);
		return true;
	}

	else 
		return false;
}

bool Hooking::removeHook()
{
	DWORD dwProtect;
	if (VirtualProtect(Hooking::funcPtr, 6, PAGE_EXECUTE_READWRITE, &dwProtect))
	{
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)Hooking::funcPtr, Hooking::origData, 6, 0);
		VirtualProtect(Hooking::funcPtr, 6, dwProtect, NULL);
		return true;
	}
	else 
		return false;
}

bool Hooking::setHookAtAddress(void* funcPtr, void* hook)
{
	Hooking::funcPtr = funcPtr;
	BYTE jmp[6] = { 0xE9, //jmp
		0x00, 0x00, 0x00, 0x00,  //address
		0xC3 //retn 
	};

	DWORD dwProtect;
	Hooking::hookData = nullptr;
	if (VirtualProtect(funcPtr, 6, PAGE_EXECUTE_READWRITE, &dwProtect)) // make memory writable
	{

		ReadProcessMemory(GetCurrentProcess(), (LPVOID)funcPtr, Hooking::origData, 6, 0); // save old data
		DWORD offset = ((DWORD)hook - (DWORD)funcPtr - 5);  //((to)-(from)-5)
		memcpy(&jmp[1], &offset, 4); // write address into jmp
		memcpy(Hooking::hookData, jmp, 6); // save hook data
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)funcPtr, jmp, 6, 0); // write jmp
		VirtualProtect(funcPtr, 6, dwProtect, NULL); // reprotect

		return true;
	}

	else 
		return false;
}

Hooking g_Hooking;