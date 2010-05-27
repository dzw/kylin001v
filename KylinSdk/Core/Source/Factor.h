#pragma once

#include "Entity.h"
#include "property.h"

namespace Kylin
{
	class Factor : public Kylin::Entity
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(Factor, Entity)
		Declare_Entity_Creator(Factor)

		Factor();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);

	protected:
		virtual KVOID PostSpawn();
		virtual KVOID PostDestroy();

	public:
		virtual KVOID SetHostAction(Action* pAct);
		
	protected:
		Action* m_spHostAct;
		// $HostID
		// $ActionGID
	};
}

