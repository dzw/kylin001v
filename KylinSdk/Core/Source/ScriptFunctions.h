#pragma once



namespace Script
{
//tolua_begin
	
	//-----------------------------------------------------
	// ������
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
	
	// ����Entity����
	extern void set_next_anim(unsigned int uEntID, const char* sAnim, float fTimes);
	// ����Entity��Ч
	extern void set_effect( unsigned int uEntID, unsigned int uEffectID);
	// ����ȫ����Ч
	extern void set_global_effect( unsigned int uEffectID );
	// ������
	extern void bind_weapon( unsigned int uEntID, unsigned int uWeaponID, const char* sNode );
	// ��װ
	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID );
	// Ϊ��������·����
	extern void add_pathway_pos( unsigned int uEntID, float x, float z);

//tolua_end
};


