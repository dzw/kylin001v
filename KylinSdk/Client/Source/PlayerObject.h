#pragma once

#include "character.h"

namespace Kylin
{
	class PlayerObject : public Character
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(PlayerObject, Character)
		Declare_Entity_Creator(PlayerObject)

		PlayerObject();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID PostSpawn();

		virtual KVOID PostDestroy();

	public:
		// 判断是否是本地玩家
		KBOOL	IsMyself();
		
		// 重生
		KVOID	Reborn();
		
		// 死亡
		KVOID	Dead();

	protected:
		KVOID	EV_Killed(EventPtr spEV);
		KVOID	EV_Reborn(EventPtr spEV);

	protected:
		// 复活时间
		KFLOAT	m_fRebornTime;

	};
}