#include "corepch.h"
#include "Action.h"
#include "ActionDispatcher.h"
#include "Factor.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"
#include "DataLoader.h"
#include "DataManager.h"
#include "DataItem.h"
#include "RegisterClass.h"


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

KVOID Kylin::Action::RemoveFactor( KUINT uFactorID )
{
	FactorList::iterator beg = m_kFactorList.begin();
	FactorList::iterator end = m_kFactorList.end();
	for (FactorList::iterator it=beg; it != end; it++)
	{
		if ((*it) == uFactorID)
		{
			m_kFactorList.erase(it);
			break;
		}
	}
}

KVOID Kylin::Action::Destroy()
{
	FactorList::iterator beg = m_kFactorList.begin();
	FactorList::iterator end = m_kFactorList.end();
	for (FactorList::iterator it=beg; it != end; it++)
	{
		EventPtr spEV(
			new Event(
			&ev_post_destroy, 
			Event::ev_nextframe, 
			0, 
			0, 
			NULL
			));

		KylinRoot::GetSingletonPtr()->PostMessage((*it),spEV);
	}
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

Kylin::Factor* Kylin::Action::SpawnFactor()
{
	KUINT uFactorGId = 0;
	if (!m_kProperty.GetUIntValue("$FactorID",uFactorGId))
		return NULL;
	
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("FACTOR_DB",sValue))
		return NULL;

	Kylin::DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);
	// 查询对应的因子信息
	Kylin::DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uFactorGId,dbItem))
		return NULL;
	
	PropertySet kFactorProp;
	// 获得模型
	DataItem::DataField dbField;
	if (dbItem.QueryField("MESH",dbField))
	{
		KSTR sMesh = boost::any_cast<KSTR>(dbField.m_aValue);	
		kFactorProp.SetValue("$Mesh",sMesh);
	}
	// 获得材质
	if (dbItem.QueryField("MATERIAL",dbField))
	{
		KSTR sMat = boost::any_cast<KSTR>(dbField.m_aValue);	
		kFactorProp.SetValue("$Material",sMat);
	}
	// 获得缩放
	if (dbItem.QueryField("SCALE",dbField))
	{
		KFLOAT fScale = boost::any_cast<KFLOAT>(dbField.m_aValue);	
		kFactorProp.SetValue("$Scale",fScale);
	}
	// 获得有效时间
	if (dbItem.QueryField("TIMES",dbField))
	{
		KFLOAT fTime = boost::any_cast<KFLOAT>(dbField.m_aValue);	
		kFactorProp.SetValue("$Times",fTime);
	}
	//////////////////////////////////////////////////////////////////////////
	// 设置特效属性
	if (dbItem.QueryField("EFFECT_ID",dbField))
	{
		KINT nEffectID = boost::any_cast<KINT>(dbField.m_aValue);	
		
		KSTR sValue;
		if (DataManager::GetSingletonPtr()->GetGlobalValue("EFFECT_DB",sValue))
		{
			Kylin::DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);
			// 查询对应的因子信息
			Kylin::DataItem dbItem;
			if (pLoader->GetDBPtr()->Query(uFactorGId,dbItem))
			{
				DataItem::DataField dbField;
				if (dbItem.QueryField("TEMPLATE",dbField))
				{
					KSTR sEffect = boost::any_cast<KSTR>(dbField.m_aValue);	
					kFactorProp.SetValue("$Effect",sEffect);
				}
			}
		}
	}

	kFactorProp.SetValue("$CLASS_ID",KUINT(id_factor));
	//////////////////////////////////////////////////////////////////////////
	// 生成因子实体
	Factor* pFactor = BtStaticCast(Factor,KylinRoot::GetSingletonPtr()->SpawnEntity(kFactorProp));
	
	SAFE_CALL(pFactor,SetHostAction(this));

	m_kFactorList.push_back(pFactor->GetID());

	return pFactor;
}

