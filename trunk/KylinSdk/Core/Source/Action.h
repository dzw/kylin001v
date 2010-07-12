#pragma once

#include "Property.h"
#include "RegisterClass.h"

//tolua_begin
// ����ʹ�÷�ʽ
enum ActionType
{
	AT_IMM,			// ��ʱ��
	AT_POS,			// λ����
	AT_TAR,			// Ŀ����
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


