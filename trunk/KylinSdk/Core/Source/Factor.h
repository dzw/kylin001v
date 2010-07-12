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
		virtual KVOID SetTarget(KUINT uTarget);
		virtual KUINT GetTarget();

	protected:
		virtual KVOID PostSpawn();
		virtual KVOID PostDestroy();
		virtual KVOID BindEffect(PropertySet kProp);

	public:
		virtual KVOID SetHostAction(Action* pAct);
		
	protected:
		Action* m_spHostAct;
		KBOOL	m_bNeedCallback;		// 是否需要回调，通知上层
		// $HostID
		// $ActionGID
	};
}

