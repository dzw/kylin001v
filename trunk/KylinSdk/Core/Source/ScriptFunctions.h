#pragma once



namespace Script
{
//tolua_begin
	
	//-----------------------------------------------------
	// 雾设置
	extern void set_fog_color(float r, float g, float b);
	extern void set_fog_distance(float d);
	extern void set_fog_intensity(float i);
	extern void set_fog_near(float n);
	extern void set_fog_far(float f);
	extern void set_fog_enable(bool b);
	
	//-----------------------------------------------------
	extern void set_camera_near(float n);
	extern void set_camera_far(float f);
	//extern void set_rander_distance(float d);
	extern void set_visible_distance(float d);
	extern void set_viewport_back(float r, float g, float b);

	//-----------------------------------------------------
	extern void lua_break(const char * sDebugInfo);
	
	// 设置Entity动画
	extern void set_next_anim(unsigned int uEntID, const char* sAnim, float fTimes);
	// 设置Entity特效
	extern void set_effect( unsigned int uEntID, unsigned int uEffectID);
	// 设置全局特效
	extern void set_global_effect( unsigned int uEffectID );
	// 换装
	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID );
	// 为非玩家添加路径点
	extern void add_pathway_pos( unsigned int uEntID, float x, float z);

//tolua_end
};


