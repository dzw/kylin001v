#pragma once



namespace Script
{
//tolua_begin
	
	//-----------------------------------------------------
	extern void lua_break(const char * sDebugInfo);
	
	// ����Entity����
	extern void set_next_anim( unsigned int uEntID, const char* sAnim, float fTimes );
	// ����Entity��Ч
	extern void set_effect( unsigned int uEntID, unsigned int uEffectID );
	// ����ȫ����Ч
	extern void set_global_effect( unsigned int uEffectID );
	// ��װ
	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID );
	// Ϊ��������·����
	extern void add_pathway_pos( unsigned int uEntID, float x, float z );
	// ���������Լ�����ĵ���
	extern void query_near_foeman( unsigned int uEntID, float r );

//tolua_end
};


