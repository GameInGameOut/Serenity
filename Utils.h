#pragma once
#include "SDK.h"

class CUtils {
public:
	void Print(const char* fmt, ...);
	DWORD FindPatternIDA(std::string moduleName, std::string pattern);
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);
}; extern CUtils* g_Utils;