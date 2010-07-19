#pragma once


namespace Script
{
	//tolua_begin

	extern void ui_reg_char_info(unsigned int uEntID);
	// 玩家学习技能
	extern void to_learn_skill(unsigned int uEntID,unsigned int uActID,char* chPos);
	// 绑定武器
	extern void to_bind_weapon( unsigned int uEntID, unsigned int uWeaponID, const char* sNode );

	// 设置默认左键技能
	extern void set_default_action(unsigned int uActID);
	
	// 
	extern void set_default_action_factory(unsigned int uEntID);

	// 更新NPC 生命UI
	extern void set_ui_monster_hp(unsigned int uEntID);
	// 更新PLAYER 生命UI
	extern void set_ui_player_hp(unsigned int uEntID);
	
	// 通知游戏结果
	extern void post_gameresult(bool bFlag);

	// 为非玩家添加路径点
	extern void set_pathway( unsigned int uEntID, unsigned int uPathwayID );

	//tolua_end
};


