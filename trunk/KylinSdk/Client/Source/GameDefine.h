#pragma once


// ��ϵ
enum ENUM_RELATION
{
	RELATION_INVALID	= -1,
	RELATION_ENEMY,				// �ж�
	RELATION_FRIEND,			// �Ѻ�

	RELATION_NUMBERS
};


enum WORLD_TIME
{
	WT_CHT_1	=	0,	//��ʱ
	WT_CHT_2,			//��ʱ
	WT_CHT_3,			//��ʱ
	WT_CHT_4,			//îʱ
	WT_CHT_5,			//��ʱ
	WT_CHT_6,			//��ʱ
	WT_CHT_7,			//��ʱ
	WT_CHT_8,			//δʱ
	WT_CHT_9,			//��ʱ
	WT_CHT_10,			//��ʱ
	WT_CHT_11,			//��ʱ
	WT_CHT_12			//��ʱ
};

enum ENUM_NPC_AI_ATTR
{
	NPC_AI_TYPE_INVALID			= -1,	// INVALID
	NPC_AI_TYPE_AFRMYFYB		= 0,	// ����������������ƶ������Թ�����Ѳ�ߣ����ỹ��
	NPC_AI_TYPE_UFNMYFYB		= 1,	// ������������������ƶ������Թ������ỹ��
	NPC_AI_TYPE_UFRMNFNB		= 2,	// ������������������ƶ��������Թ��������ỹ��
	NPC_AI_TYPE_UFNMNFNB		= 3,	//������������������ƶ��������Թ��������ỹ��
	NPC_AI_TYPE_AFRMYFYB		= 4,	//����������������ƶ������Թ������ỹ������Ѳ�ߣ�
	NPC_AI_TYPE_IS_5			= 5,	//������������������ƶ������Թ������ỹ��
	NPC_AI_TYPE_IS_6			= 6,	//��������������������ƶ������Թ������ỹ��
	NPC_AI_TYPE_IS_7			= 7,	//��������������������ƶ������Թ��������ỹ��
	NPC_AI_TYPE_IS_8			= 8,	//������������������ƶ������Թ��������ỹ��
	NPC_AI_TYPE_IS_9			= 9,	//������������������ƶ������Թ������ỹ��������ר�ã�Сɨ�а뾶��
};