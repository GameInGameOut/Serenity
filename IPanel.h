#include "SDK.h"

class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char *(__thiscall* tGetName)(void*, unsigned int);
		return VT::VFunc<tGetName>(this, 36)(this, vguiPanel);
	}
#ifdef GetClassName
#undef GetClassName
#endif
	const char *GetClassName(unsigned int vguiPanel)
	{
		typedef const char *(__thiscall* tGetClassName)(void*, unsigned int);
		return VT::VFunc<tGetClassName>(this, 37)(this, vguiPanel);
	}
}; extern IPanel* i_Panel;