#pragma once



namespace Script
{
//tolua_begin
	
	extern void lua_break(const char * sDebugInfo);
	
	// ����Entity����
	extern void set_next_anim(unsigned int uEntID, const char* sAnim, float fTimes);
	// ����Entity��Ч
	extern void set_effect( unsigned int uEntID, unsigned int uEffectID);
	// ����ȫ����Ч
	extern void set_global_effect( unsigned int uEffectID );

//tolua_end
};


