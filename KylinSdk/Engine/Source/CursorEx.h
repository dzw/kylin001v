#pragma once

#include "GuiManager.h"

namespace Kylin
{
	enum ENUM_CURSOR_TYPE
	{
		CURSOR_WINBASE = 0,

		CURSOR_NORMAL,			//普通
		CURSOR_ATTACK,			//攻击
		CURSOR_AUTORUN,			//自动行走中
		CURSOR_PICKUP,			//拾取物品
		CURSOR_UNREACHABLE,		//无法到达的区域
		CURSOR_MINE,			//采矿
		CURSOR_HERBS,			//采药
		CURSOR_FISH,			//钓鱼
		CURSOR_SPEAK,			//谈话
		CURSOR_INTERACT,		//齿轮
		CURSOR_REPAIR,			//修理
		CURSOR_HOVER,			//鼠标激活(挂接物品...)
		CURSOR_IDENTIFY,		//鼠标鉴定
		CURSOR_ADDFRIEND,		//添加好友
		CURSOR_EXCHANGE,		//添加好友
		CURSOR_CATCH_PET,		//捉充

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