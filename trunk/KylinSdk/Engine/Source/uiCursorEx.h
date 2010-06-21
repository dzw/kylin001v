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

			CT_NORMAL,			//普通
			CT_ATTACK,			//攻击
			CT_AUTORUN,			//自动行走中
			CT_PICKUP,			//拾取物品
			CT_UNREACHABLE,		//无法到达的区域
			CT_MINE,			//采矿
			CT_HERBS,			//采药
			CT_FISH,			//钓鱼
			CT_SPEAK,			//谈话
			CT_INTERACT,		//齿轮
			CT_REPAIR,			//修理
			CT_HOVER,			//鼠标激活(挂接物品...)
			CT_IDENTIFY,		//鼠标鉴定
			CT_ADDFRIEND,		//添加好友
			CT_EXCHANGE,		//添加好友
			CT_CATCH_PET,		//捕捉

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