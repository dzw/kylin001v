#pragma once


namespace Script
{
	//tolua_begin

	extern void ui_reg_char_info(unsigned int uEntID);
	// ѧϰ����
	extern void to_learn_skill(unsigned int uEntID,unsigned int uActID,char* chPos);
	// ������
	extern void to_bind_weapon( unsigned int uEntID, unsigned int uWeaponID, const char* sNode );

	//tolua_end
};


