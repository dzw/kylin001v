#include "cltpch.h"
#include "ClActionFactory.h"
#include "action.h"
#include "datamanager.h"
#include "DataLoader.h"
#include "property.h"
#include "ActSkill.h"
#include "DataItem.h"


Kylin::Action* Kylin::ClActionFactory::Generate( KUINT uGID )
{
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("ACTION_DB",sValue))
		return false;

	Kylin::DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);
	// 查询对应的角色信息
	Kylin::DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uGID,dbItem))
		return false;

	DataItem::DataField dbField;
	dbItem.QueryField("CLASS",dbField);
	KSTR sClass = boost::any_cast<KSTR>(dbField.m_aValue);
	
	PropertySet kProp;
	kProp.SetValue("$GID",uGID);
	kProp.SetValue("$FactorID",KUINT(1));
	//////////////////////////////////////////////////////////////////////////
	Action* pAct = NULL;
	if (sClass == "ActSkill")
	{
		pAct = KNEW ActSkill(m_pDispatcher);
		pAct->Init(kProp);
	}


	return pAct;
}