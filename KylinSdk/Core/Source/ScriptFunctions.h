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
	extern void add_effect( unsigned int uEntID, unsigned int uEffectID , float fTimes);
	// 加入全局特效
	extern void add_global_effect( unsigned int uEffectID );
	
	// 换装
	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID );
	
	// 搜索距离自己最近的敌人
	extern void query_near_foeman( unsigned int uEntID, float r );

	// 设置spawner参数
	extern void set_spawner(unsigned int uEntID, float fInterval, float fDelay, int nMaxCount, unsigned int uSpawnID, bool bFlag);

	// 设置位置
	extern void set_translate(unsigned int uEntID, float x, float z);
	
	// 设置计时器
	extern void set_timer(unsigned int uEntID, float fTimeStep);
	// 销毁计时器
	extern void kill_timer(unsigned int uEntID);

	// 杀死entity
	extern void kill_character(unsigned int uEntID, unsigned int uKiller);
	
	// 销毁entity
	extern void destroy_entity(unsigned int uEntID , float fTimeDelay);
	
	// 抛出道具
	extern void throw_item(unsigned int uEntID, unsigned int uItem, float fTimeDelay);
	
	// 从运动到静止
	extern void to_stop(unsigned int uEntID);
	
	// 切换背景音乐
	extern void change_background_sound(unsigned int uSoundID);

//tolua_end
};


