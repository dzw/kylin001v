#include "CorePCH.h"
#include ".\levelcontroller.h"
#include "RegisterClass.h"
#include "scriptvm.h"

#include "rOgreRoot.h"
#include "KylinRoot.h"

//////////////////////////////////////////////////////////////////////////
//
KVOID Kylin::LevelEventReg::RegisterEvent( KUINT id, const EventTemplate* pEV)
{
	EventMapping::iterator it;
	it = m_mapping.find(id);
	if(it == m_mapping.end())
	{
		TemplateVector a;
		a.push_back(pEV);
		m_mapping.insert(std::pair<KUINT, TemplateVector>(id, a));
	}
	else
	{
		it->second.push_back(pEV);
	}
}

KVOID Kylin::LevelEventReg::RegisterEvent( KUINT id, KCCHAR * sName)
{
	const EventTemplate * pTemp = EventTemplate::Find(sName);
	assert( pTemp );

	if(pTemp!=NULL)
		RegisterEvent(id, pTemp);
	else
	{
		//syslog->debug("Found Invalid event template: %s\n", sName);
	}
}

KVOID Kylin::LevelEventReg::UnRegisterEvent( KUINT id, const EventTemplate* pEV)
{
	EventMapping::iterator it = m_mapping.find(id);
	if( it != m_mapping.end())
	{
		TemplateVector& v = it->second;
		for( TemplateVector::iterator it1 = v.begin(); it1!=v.end(); ++it1)
		{
			if(*it1 == pEV)
			{
				v.erase(it1);
				break;
			}
		}
	}
}

KVOID Kylin::LevelEventReg::UnRegisterEvent( KUINT id, KCCHAR * sName)
{
	const EventTemplate * pTemp = EventTemplate::Find(sName);
	if(pTemp!=NULL)
		UnRegisterEvent(id, pTemp);
	//else
	//	syslog->debug("Found Invalid event template: %s\n", sName);
}

KBOOL Kylin::LevelEventReg::HasRegisteredEvent( KUINT id, const EventTemplate* pEV)
{
	EventMapping::iterator it = m_mapping.find(id);
	if( it != m_mapping.end())
	{
		TemplateVector& v = it->second;
		for( TemplateVector::iterator it1 = v.begin(); it1!=v.end(); ++it1)
		{
			if(*it1 == pEV)
			{
				return true;
			}
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
//
KVOID Kylin::LevelController::Tick( float fElapsed )
{

}

KVOID Kylin::LevelController::HandleEntityInit( Entity * pEntity )
{

}

KVOID Kylin::LevelController::EV_LVLCommonEvent( EventPtr ev )
{
	KUINT id = boost::get<unsigned int>(ev->args[0]);
	Kylin::Entity * pEntity = KylinRoot::GetSingletonPtr()->GetEntity(id);
	assert(pEntity);

	//if this entity has registered level update?
	ExecuteScriptFunc(pEntity, ev->m_pTemplate, "i", pEntity->GetID());
}

KVOID Kylin::LevelController::EV_LVLInit( EventPtr ev )
{
	KUINT id = boost::get<KUINT>(ev->args[0]);
	Kylin::Entity * pEntity = KylinRoot::GetSingletonPtr()->GetEntity(id);
	if(pEntity)
	{
		KVEC<KCCHAR *>modules;
		modules.push_back("level");
		modules.push_back(pEntity->GetName());
		
		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(modules, ev->m_pTemplate->GetName(), true, "i", pEntity->GetID());
	}
}

KVOID Kylin::LevelController::ExecuteScriptFunc( Entity * pEnt, const EventTemplate * pTemplate, KCCHAR * sig, ... )
{
	if(m_levelReg.HasRegisteredEvent(pEnt->GetID(), pTemplate))
	{
		KVEC<KCCHAR *>modules;
		modules.push_back("level");
		modules.push_back(pEnt->GetName());
		va_list vl;
		va_start(vl, pTemplate);
		OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(modules, pTemplate->GetName(), false, sig, vl);
		va_end(vl);
	}
}

KVOID Kylin::LevelController::ExecuteScriptFunc( Entity * pEnt, KCCHAR * sScriptName, KCCHAR * sig, ... )
{
	KVEC<KCCHAR *>modules;
	modules.push_back("level");
	modules.push_back(pEnt->GetName());
	va_list vl;
	va_start(vl, sig);
	OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(modules, sScriptName, false, sig, vl);
	va_end(vl);
}

KVOID Kylin::LevelController::ExecuteScriptFunc1Param( Entity * pEnt, KCCHAR * sScriptName, KCCHAR * sig ,KINT nParam )
{
	KVEC<KCCHAR *>modules;
	modules.push_back("level");
	modules.push_back(pEnt->GetName());
	OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(modules, sScriptName, false, sig, nParam);
}

