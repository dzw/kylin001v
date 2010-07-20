#include "cltpch.h"
#include "ClActionFactory.h"
#include "action.h"
#include "datamanager.h"
#include "DataLoader.h"
#include "property.h"
#include "ActSkill.h"
#include "SummonSkill.h"
#include "FoodSkill.h"
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

	dbItem.QueryField("TYPE",dbField);
	KSTR sType = boost::any_cast<KSTR>(dbField.m_aValue);	

	ActionType eType = Action::TransformType(sType);

	dbItem.QueryField("FACTOR_ID",dbField);
	KUINT uFactor = boost::any_cast<KINT>(dbField.m_aValue);	

	dbItem.QueryField("ANIMATION",dbField);
	KSTR sAnim = boost::any_cast<KSTR>(dbField.m_aValue);	
	
	dbItem.QueryField("ICON",dbField);
	KSTR sIcon = boost::any_cast<KSTR>(dbField.m_aValue);	

	dbItem.QueryField("COOLDOWN",dbField);
	KFLOAT fCooldown = boost::any_cast<KFLOAT>(dbField.m_aValue);	

	dbItem.QueryField("RANGE",dbField);
	KFLOAT fRange = boost::any_cast<KFLOAT>(dbField.m_aValue);	

	dbItem.QueryField("MAX_DAMAGE",dbField);
	KINT fMaxDamage = boost::any_cast<KINT>(dbField.m_aValue);	

	dbItem.QueryField("MIN_DAMAGE",dbField);
	KINT fMinDamage = boost::any_cast<KINT>(dbField.m_aValue);	

	dbItem.QueryField("EXPLAIN",dbField);
	KSTR sExplain = boost::any_cast<KSTR>(dbField.m_aValue);	

	PropertySet kProp;
	kProp.SetValue("$GID",uGID);
	kProp.SetValue("$Type",(KUINT)eType);
	kProp.SetValue("$FactorID",uFactor);
	kProp.SetValue("$Animation",sAnim);
	kProp.SetValue("$Icon",sIcon);
	kProp.SetValue("$Cooldown",fCooldown);
	kProp.SetValue("$Range",fRange);
	kProp.SetValue("$MaxDamage",fMaxDamage);
	kProp.SetValue("$MinDamage",fMinDamage);
	kProp.SetValue("$Explain",sExplain);
	//////////////////////////////////////////////////////////////////////////
	//
	Action* pAct = NULL;
	if (sClass == "ActSkill")
	{
		pAct = KNEW ActSkill(m_pDispatcher);
	}
	else if (sClass == "FoodSkill")
	{
		pAct = KNEW FoodSkill(m_pDispatcher);
	}
	else if (sClass == "SummonSkill")
	{
		pAct = KNEW SummonSkill(m_pDispatcher);
	}

	pAct->Init(kProp);

	return pAct;
}