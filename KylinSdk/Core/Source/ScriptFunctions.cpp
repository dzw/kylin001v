#include "corepch.h"

#include ".\scriptfunctions.h"
#include "ScriptVM.h"


namespace Script
{

	void lua_break(const char * sDebugInfo)
	{
		OutputDebugString(sDebugInfo);
		DebugBreak();
	}
	

	void lua_test(int i)
	{
		i = 0;
	}
}