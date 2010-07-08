#pragma once


namespace Script
{
	//tolua_begin

	extern void ui_reg_char_info(unsigned int uEntID);
	// 学习技能
	extern void to_learn_skill(unsigned int uEntID,unsigned int uActID,char* chPos);
	// 绑定武器
	extern void to_bind_weapon( unsigned int uEntID, unsigned int uWeaponID, const char* sNode );
	// 设置默认左手技能
	extern void set_default_action(unsigned int uActID);
	//tolua_end
};


