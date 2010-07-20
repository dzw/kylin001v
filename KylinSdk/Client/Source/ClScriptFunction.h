#pragma once


namespace Script
{
	//tolua_begin

	extern void ui_reg_char_info(unsigned int uEntID);
	// ���ѧϰ����
	extern void to_learn_skill(unsigned int uEntID,unsigned int uActID,char* chPos);
	// ������
	extern void to_bind_weapon( unsigned int uEntID, unsigned int uWeaponID, const char* sNode );

	// ����Ĭ���������
	extern void set_default_action(unsigned int uActID);
	
	// 
	extern void set_default_action_factory(unsigned int uEntID);

	// ����NPC ����UI
	extern void set_ui_monster_hp(unsigned int uEntID);
	// ����PLAYER ����UI
	extern void set_ui_player_hp(unsigned int uEntID);
	
	// ֪ͨ��Ϸ���
	extern void post_gameresult(bool bFlag);

	// Ϊ��������·����
	extern void set_pathway( unsigned int uEntID, unsigned int uPathwayID );
	// �������
	extern void add_item(unsigned int uEntID, unsigned int uItemGID);

	//tolua_end
};


