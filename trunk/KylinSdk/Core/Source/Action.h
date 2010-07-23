#pragma once

#include "Property.h"
#include "RegisterClass.h"
#include "ClockingCallback.h"

//tolua_begin
// 技能使用方式
enum ActionType
{
	AT_IMM,			// 及时型
	AT_POS,			// 位置型
	AT_TAR,			// 目标型
};
//tolua_end

namespace Kylin
{
	class Action : public ClockingCallback
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
		
		PropertySet&	GetPropertyRef() { return m_kProperty; }

		KUINT	GetGID();
		
		KSTR	GetIcon();
		KSTR	GetExplain();
		KINT	GetMaxDamage();
		KINT	GetMinDamage();
		KFLOAT	GetRange();
		KFLOAT	GetCooldawn();

		KUINT	GetHostWorldID();
		
		static ActionType TransformType(KCSTR& sType);

	public:
		KVOID			SetEmitterNode(Ogre::Node* pNode);

	protected:
		virtual KVOID	OnTriggered(Factor* pFactor);
		virtual KVOID	EndTime(KCSTR& sClass,KCSTR& sName, KANY aUserData);

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


