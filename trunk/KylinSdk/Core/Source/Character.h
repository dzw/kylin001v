#pragma once

#include "Entity.h"
#include "inputlistener.h"


namespace Kylin
{
	class Character : public Entity
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(Character, Entity)
		Declare_Entity_Creator(Character)

		Character();
		
		KVOID	SetActionFactory(ActionFactory*);
		ActionDispatcher* GetActionDispatcher();
		
		Avatar*	GetAvatar();
		Kitbag*	GetKitbag();
		
		enum LifeStatus
		{
			LS_NONE,
			LS_ALIVE,
			LS_DEAD,
		};
		
		KBOOL	IsDead() { return m_eLifeStatus != LS_ALIVE; }

	protected:
		KVOID	EV_Damage(EventPtr spEV);
		KVOID	EV_ThrowItem(EventPtr spEV);

	protected:
		virtual KVOID OnEntityCllsn(Entity* pCollidee,const KPoint3& rNormal);
		virtual KBOOL OnShouldCllsn(Entity* pCollidee);
		
		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID PostSpawn();
		
		virtual KVOID PostDestroy();

	protected:
		// multiple player...
		KBOOL	IsMyself(){ return true; }
		
		// ��������
	protected:

		// ����״��
		LifeStatus			m_eLifeStatus;

		// װ����Ϣ
		Avatar*				m_pAvatar;
		
		// ����
		Kitbag*				m_pKitbag;

		// ��Ϊ�ɷ�
		ActionDispatcher*	m_pActDispatcher;
	};
}
