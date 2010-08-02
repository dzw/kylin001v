#pragma once


namespace Script
{
	//tolua_begin

	extern void ui_reg_char_info(unsigned int uEntID);
	// 玩家学习技能
	extern void to_learn_skill(unsigned int uEntID,unsigned int uActID,bool toUi=false);
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
	extern void set_ui_player_exp(unsigned int uEntID);

	// 通知游戏结果
	extern void post_gameresult(bool bFlag);

	// 为非玩家添加路径点
	extern void set_pathway( unsigned int uEntID, unsigned int uPathwayID );
	// 加入道具
	extern void add_item(unsigned int uEntID, unsigned int uItemGID);
	
	// 注册到小地图
	extern void addin_minimap(unsigned int uEntID);
	
	// 累加成功条件
	extern void add_success_factor();

	// 累加失败条件
	extern void add_failure_factor();
	
	// 获得成功条件
	extern void set_success_factors(int nCount);

	// 获得失败条件
	extern void set_failure_factors(int nCount);
	
	// 设置任务说明
	extern void set_task_explain(const char* pExplain);

	//tolua_end
};


