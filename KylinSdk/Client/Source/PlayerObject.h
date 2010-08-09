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
		// �ж��Ƿ��Ǳ������
		KBOOL	IsMyself();
		
		// ����
		KVOID	Reborn();
		
		// ����
		KVOID	Dead();

	protected:
		KVOID	EV_Killed(EventPtr spEV);
		KVOID	EV_Reborn(EventPtr spEV);
		KVOID	EV_RestoreHP(EventPtr spEV);
		KVOID	EV_LearnSkill(EventPtr spEV);			
		KVOID	Ev_Teleport(EventPtr spEV);

	protected:
		// ����ʱ��
		KFLOAT	m_fRebornTime;

	};
}