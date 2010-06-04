#pragma once



namespace Script
{
//tolua_begin
	
	extern void lua_break(const char * sDebugInfo);
	
	// 设置Entity动画
	extern void set_next_anim(unsigned int uEntID, const char* sAnim, float fTimes);
	// 设置Entity特效
	extern void set_effect( unsigned int uEntID, unsigned int uEffectID);
	// 设置全局特效
	extern void set_global_effect( unsigned int uEffectID );
	// 绑定武器
	extern void bind_weapon( unsigned int uEntID, unsigned int uWeaponID, const char* sNode );
	// 换装
	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID );

//tolua_end
};


