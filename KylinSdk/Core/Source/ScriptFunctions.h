#pragma once



namespace Script
{
//tolua_begin
	
	//-----------------------------------------------------
	extern void lua_break(const char * sDebugInfo);
	
	// ����Entity����
	extern void set_next_anim( unsigned int uEntID, const char* sAnim, float fTimes );
	// ���붯������(��ѭ������)
	extern void add_anim_queue( unsigned int uEntID, const char* sAnim );

	// ��Entity������Ч
	extern void add_effect( unsigned int uEntID, unsigned int uEffectID );
	// ����ȫ����Ч
	extern void add_global_effect( unsigned int uEffectID );
	
	// ��װ
	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID );
	
	// Ϊ��������·����
	extern void add_pathway_pos( unsigned int uEntID, float x, float z );

	// ���������Լ�����ĵ���
	extern void query_near_foeman( unsigned int uEntID, float r );

	// ����spawner����
	extern void set_spawner(unsigned int uEntID, float fInterval, float fDelay, int nMaxCount, unsigned int uSpawnID, bool bFlag);

	// ����λ��
	extern void set_translate(unsigned int uEntID, float x, float z);
	
	// ���ü�ʱ��
	extern void set_timer(unsigned int uEntID, float fTimeStep);
	// ���ټ�ʱ��
	extern void kill_timer(unsigned int uEntID);

	// ɱ��entity
	extern void kill_character(unsigned int uEntID, unsigned int uKiller);
	
	// ����entity
	extern void destroy_entity(unsigned int uEntID , float fTimeDelay);
	
	// �׳�����
	extern void throw_item(unsigned int uEntID, unsigned int uItem, float fTimeDelay);

//tolua_end
};


