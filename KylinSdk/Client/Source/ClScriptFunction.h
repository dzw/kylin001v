#pragma once


namespace Script
{
	//tolua_begin

	extern void ui_reg_char_info(unsigned int uEntID);
	// ѧϰ����
	extern void to_learn_skill(unsigned int uEntID,unsigned int uActID,char* chPos);
	// ������
	extern void to_bind_weapon( unsigned int uEntID, unsigned int uWeaponID, const char* sNode );
	// ����Ĭ�����ּ���
	extern void set_default_action(unsigned int uActID);
	//tolua_end
};


