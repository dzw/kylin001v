#pragma once



namespace Script
{
//tolua_begin
	
	//-----------------------------------------------------
	extern void lua_break(const char * sDebugInfo);
	
	// 设置Entity动画
	extern void set_next_anim( unsigned int uEntID, const char* sAnim, float fTimes );
	// 设置Entity特效
	extern void set_effect( unsigned int uEntID, unsigned int uEffectID );
	// 设置全局特效
	extern void set_global_effect( unsigned int uEffectID );
	// 换装
	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID );
	// 为非玩家添加路径点
	extern void add_pathway_pos( unsigned int uEntID, float x, float z );
	// 搜索距离自己最近的敌人
	extern void query_near_foeman( unsigned int uEntID, float r );

//tolua_end
};


