#pragma once

//tolua_begin
// 技能使用方式
enum USE_SKILL_TYPE
{
	IMM,			// 及时型
	POS,			// 位置型
	TAR,			// 目标型
};
//tolua_end

// 关系
enum ENUM_RELATION
{
	RELATION_INVALID	= -1,
	RELATION_ENEMY,				// 敌对
	RELATION_FRIEND,			// 友好

	RELATION_NUMBERS
};


enum WORLD_TIME
{
	WT_CHT_1	=	0,	//子时
	WT_CHT_2,			//丑时
	WT_CHT_3,			//寅时
	WT_CHT_4,			//卯时
	WT_CHT_5,			//辰时
	WT_CHT_6,			//巳时
	WT_CHT_7,			//午时
	WT_CHT_8,			//未时
	WT_CHT_9,			//申时
	WT_CHT_10,			//酉时
	WT_CHT_11,			//戌时
	WT_CHT_12			//亥时
};

enum ENUM_NPC_AI_ATTR
{
	NPC_AI_TYPE_INVALID			= -1,	// INVALID
	NPC_AI_TYPE_AFRMYFYB		= 0,	// 主动攻击，会随机移动，可以攻击（巡逻），会还击
	NPC_AI_TYPE_UFNMYFYB		= 1,	// 不主动攻击，会随机移动，可以攻击，会还击
	NPC_AI_TYPE_UFRMNFNB		= 2,	// 不主动攻击，会随机移动，不可以攻击，不会还击
	NPC_AI_TYPE_UFNMNFNB		= 3,	//不主动攻击，不随机移动，不可以攻击，不会还击
	NPC_AI_TYPE_AFRMYFYB		= 4,	//主动攻击，会随机移动，可以攻击，会还击（不巡逻）
	NPC_AI_TYPE_IS_5			= 5,	//主动攻击，不会随机移动，可以攻击，会还击
	NPC_AI_TYPE_IS_6			= 6,	//不主动攻击，不会随机移动，可以攻击，会还击
	NPC_AI_TYPE_IS_7			= 7,	//不主动攻击，不会随机移动，可以攻击，不会还击
	NPC_AI_TYPE_IS_8			= 8,	//不主动攻击，会随机移动，可以攻击，不会还击
	NPC_AI_TYPE_IS_9			= 9,	//主动攻击，不会随机移动，可以攻击，会还击（副本专用，小扫敌半径）
};