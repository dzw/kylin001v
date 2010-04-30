#pragma once

#include "GuiManager.h"

namespace Kylin
{
	enum ENUM_CURSOR_TYPE
	{
		CURSOR_WINBASE = 0,

		CURSOR_NORMAL,			//��ͨ
		CURSOR_ATTACK,			//����
		CURSOR_AUTORUN,			//�Զ�������
		CURSOR_PICKUP,			//ʰȡ��Ʒ
		CURSOR_UNREACHABLE,		//�޷����������
		CURSOR_MINE,			//�ɿ�
		CURSOR_HERBS,			//��ҩ
		CURSOR_FISH,			//����
		CURSOR_SPEAK,			//̸��
		CURSOR_INTERACT,		//����
		CURSOR_REPAIR,			//����
		CURSOR_HOVER,			//��꼤��(�ҽ���Ʒ...)
		CURSOR_IDENTIFY,		//������
		CURSOR_ADDFRIEND,		//��Ӻ���
		CURSOR_EXCHANGE,		//��Ӻ���
		CURSOR_CATCH_PET,		//׽��

		CURSOR_NUMBER,
	};

	class CursorEx : public GuiBase
	{
	
	public:	
		virtual KBOOL Initialize(){ return true; }
		virtual KVOID Render(KFLOAT fElapsed){}
		virtual KVOID Destroy(){}

		virtual KVOID SetVisible(KBOOL bVisible);

	};
}