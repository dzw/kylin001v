#pragma once

#include "GuiManager.h"

namespace Kylin
{
	class CursorEx : public GuiBase
	{
	public:	
		CursorEx();

		enum CursorType
		{
			CT_WINBASE = 0,

			CT_NORMAL,			//��ͨ
			CT_ATTACK,			//����
			CT_AUTORUN,			//�Զ�������
			CT_PICKUP,			//ʰȡ��Ʒ
			CT_UNREACHABLE,		//�޷����������
			CT_MINE,			//�ɿ�
			CT_HERBS,			//��ҩ
			CT_FISH,			//����
			CT_SPEAK,			//̸��
			CT_INTERACT,		//����
			CT_REPAIR,			//����
			CT_HOVER,			//��꼤��(�ҽ���Ʒ...)
			CT_IDENTIFY,		//������
			CT_ADDFRIEND,		//��Ӻ���
			CT_EXCHANGE,		//��Ӻ���
			CT_CATCH_PET,		//��׽

			CT_NUMBER,
		};


		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed){}
		virtual KVOID Destroy(){}

		virtual KVOID SetVisible(KBOOL bVisible);
		
		virtual KVOID SetPointer(CursorType eType);

		virtual CursorType GetPointerType();

	protected:
		CursorType	m_eType;
	};
}