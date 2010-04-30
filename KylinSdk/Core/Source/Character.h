#pragma once

#include "Entity.h"
#include "inputlistener.h"


namespace Kylin
{
	class Character : public Entity , public InputListener
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(Character, Entity)
		Declare_Entity_Creator(Character)

		Character();
		virtual ~Character();
		
	protected:
		KVOID	EV_Picked(EventPtr spEV);
		KVOID	EV_PickTerrain(EventPtr spEV);

		virtual KVOID OnEntityCllsn(Node* pCollidee,const KPoint3& rNormal);
		virtual KBOOL OnShouldCllsn(Node* pCollidee);

	protected:
		// multiple player...
		KBOOL	IsMyself(){ return true; }

	protected:
		virtual KVOID OnLButtonDown(KINT nX, KINT nY);
		virtual KVOID OnRButtonDown(KINT nX, KINT nY);

		// ��������
	};
}
