#pragma once


enum AI_STATE
{
	AI_INVALID	= -1,
	AI_IDLE,				//������
	AI_MOVE,				//�ƶ���
	AI_JUMP,
	AI_USE_SKILL,			//����ʹ����
	
	AI_DEFAULT_EVENT,		//��npc̸��������
	AI_DEAD,				//����
	AI_FOLLOW,				//����
	
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

