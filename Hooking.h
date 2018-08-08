#pragma once

class Hooking
{
public:
	bool findFunc(char* libName, char* funcName);
	bool removeHook();
	bool reapplyHook();
	bool applyHook(void* hook);
	bool setHookAtAddress(void* funcPtr, void* hook);

private:
	void* funcPtr;
	void* hookData;
	void* origData;
}; extern Hooking g_Hooking;