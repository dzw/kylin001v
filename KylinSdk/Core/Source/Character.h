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

	protected:
		KVOID	EV_Damage(EventPtr spEV);

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
		
		// װ����Ϣ
		Avatar*				m_pAvatar;
		
		// ����
		Kitbag*				m_pKitbag;

		// ��Ϊ�ɷ�
		ActionDispatcher*	m_pActDispatcher;
	};
}
