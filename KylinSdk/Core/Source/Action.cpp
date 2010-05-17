#include "corepch.h"
#include "Action.h"
#include "ActionDispatcher.h"
#include "Factor.h"


Kylin::Action::Action( ActionDispatcher* pDispatcher )
: m_pDispatcher(pDispatcher)
{
	
}

Kylin::Action::~Action()
{
	
}

KBOOL Kylin::Action::Init( const PropertySet& kProp )
{
	m_kProperty = kProp;

	return true;
}

KVOID Kylin::Action::Tick( KFLOAT fElapsed )
{
	if (!IsComplete())
	{

	}
}

KVOID Kylin::Action::Destroy()
{
	m_kFactorList.clear();
}

KVOID Kylin::Action::OnTriggered( Factor* pFactor )
{
	
}

KBOOL Kylin::Action::IsComplete()
{
	KUINT uMinFactorCount = 0;
	m_kProperty.GetUIntValue("$MinFactorCount",uMinFactorCount);
	if (m_kFactorList.size() < uMinFactorCount)
		return false;

	return true;
}

KUINT Kylin::Action::GetGID()
{
	KUINT uGId = -1;
	m_kProperty.GetUIntValue("$GID",uGId);

	return uGId;
}
