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

	public:
		// ��λ�����õ�����
		//virtual KVOID	SetTranslateToTerrain(KPoint3 vPos);

	protected:
		KVOID	EV_Picked(EventPtr spEV);
		KVOID	EV_PickTerrain(EventPtr spEV);

		virtual KVOID OnEntityCllsn(Node* pCollidee,const KPoint3& rNormal);
		virtual KBOOL OnShouldCllsn(Node* pCollidee);
		
		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID PostSpawn();
		
		virtual KVOID Destroy();

	protected:
		// multiple player...
		KBOOL	IsMyself(){ return true; }
		
		// ��������
	protected:
		
		// װ����Ϣ
		Avatar*				m_pAvatar;

		// ��Ϊ�ɷ�
		ActionDispatcher*	m_pActDispatcher;
	};
}
