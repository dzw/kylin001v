#pragma once

#include "Property.h"
#include "RegisterClass.h"

//tolua_begin
enum ActionType
{
	AT_NONE,
	AT_POINT,
	AT_TARGET,
};
//tolua_end

namespace Kylin
{
	class Action
	{
	public:
		Action(ActionDispatcher* pDispatcher);
		virtual ~Action();

		virtual KBOOL	Init(const PropertySet& kProp);

		virtual KVOID	Tick(KFLOAT fElapsed);
		virtual KVOID	Destroy();
		
		virtual KBOOL	IsComplete();
		
		virtual Factor* SpawnFactor();
		virtual Factor* SpawnFactor(PropertySet& kFactorProp);
		virtual KVOID	RemoveFactor(KUINT uFactorID);

		virtual KUINT	GetGID();
		
		virtual KSTR	GetIcon();
		virtual KSTR	GetExplain();
		
		virtual KUINT	GetHostWorldID();
		
		static ActionType TransformType(KCSTR& sType);

	public:
		KVOID			SetEmitterNode(Ogre::Node* pNode);

	protected:
		virtual KVOID	OnTriggered(Factor* pFactor);
		
	protected:
		friend class ActionDispatcher;
		friend class Factor;

		PropertySet m_kProperty;
		ActionType	m_eType;

		typedef KLIST<KUINT> FactorList;
		FactorList m_kFactorList;

		ActionDispatcher*	m_pDispatcher;

		Ogre::Node*			m_pEmitterNode;
	};
}


