#pragma once

namespace VT
{
	template<typename T>
	__forceinline static T VFunc(void *base, int index)
	{
		DWORD *VTable = *(DWORD**)base;
		return (T)VTable[index];
	}
}
