#pragma once


enum AI_STATE
{
	AI_INVALID	= -1,
	AI_IDLE,				//空闲中
	AI_MOVE,				//移动中
	AI_JUMP,
	AI_USE_SKILL,			//技能使用中
	
	AI_DEFAULT_EVENT,		//对npc谈话请求中
	AI_DEAD,				//死亡
	AI_FOLLOW,				//跟随
	
	AI_NUMBERS
};

enum AI_COMMAND
{
	CMD_GO_
};

namespace Kylin
{
	class BaseAI
	{
	public:
		BaseAI(Character* pHost);
		
		virtual KVOID Tick(KFLOAT fElapsed);

	protected:
		Character	m_pHostChar;
	};
}

