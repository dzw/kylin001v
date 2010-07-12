#pragma once



namespace Script
{
//tolua_begin
	
	//-----------------------------------------------------
	extern void lua_break(const char * sDebugInfo);
	
	// 设置Entity动画
	extern void set_next_anim( unsigned int uEntID, const char* sAnim, float fTimes );
	// 加入动画队列(无循环动画)
	extern void add_anim_queue( unsigned int uEntID, const char* sAnim );

	// 对Entity加入特效
	extern void add_effect( unsigned int uEntID, unsigned int uEffectID );
	// 加入全局特效
	extern void add_global_effect( unsigned int uEffectID );
	
	// 换装
	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID );
	
	// 为非玩家添加路径点
	extern void add_pathway_pos( unsigned int uEntID, float x, float z );

	// 搜索距离自己最近的敌人
	extern void query_near_foeman( unsigned int uEntID, float r );

	// 设置spawner参数
	extern void set_spawner(unsigned int uEntID, float fInterval, float fDelay, int nMaxCount, unsigned int uSpawnID);

	// 设置位置
	extern void set_translate(unsigned int uEntID, float x, float z);

//tolua_end
};


