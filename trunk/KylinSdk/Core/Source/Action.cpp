#include "corepch.h"
#include "Action.h"
#include "ActionDispatcher.h"
#include "Factor.h"
#include "RemoteEvents.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "DataLoader.h"
#include "DataManager.h"
#include "DataItem.h"
#include "ScriptVM.h"
#include "FileUtils.h"
#include "rOgreUtils.h"


Kylin::Action::Action( ActionDispatcher* pDispatcher )
: m_pDispatcher(pDispatcher)
, m_eType(AT_NONE)
, m_pEmitterNode(NULL)
{
	
}
//-------------------------------------------------------------------
Kylin::Action::~Action()
{
	
}
//-------------------------------------------------------------------
KBOOL Kylin::Action::Init( const PropertySet& kProp )
{
	m_kProperty = kProp;

	return true;
}
//-------------------------------------------------------------------
KVOID Kylin::Action::Tick( KFLOAT fElapsed )
{
	if (!IsComplete())
	{

	}
}
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
KVOID Kylin::Action::OnTriggered( Factor* pFactor )
{
	
}
//-------------------------------------------------------------------
KBOOL Kylin::Action::IsComplete()
{
	KUINT uMinFactorCount = 0;
	m_kProperty.GetUIntValue("$MinFactorCount",uMinFactorCount);
	if (m_kFactorList.size() < uMinFactorCount)
		return false;

	return true;
}
//-------------------------------------------------------------------
KUINT Kylin::Action::GetGID()
{
	KUINT uGId = -1;
	m_kProperty.GetUIntValue("$GID",uGId);

	return uGId;
}
//-------------------------------------------------------------------
Kylin::Factor* Kylin::Action::SpawnFactor()
{
	KUINT uFactorGId = 0;
	if (!m_kProperty.GetUIntValue("$FactorID",uFactorGId))
		return NULL;
	
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("FACTOR_DB",sValue))
		return NULL;

	Kylin::DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);
	// ��ѯ��Ӧ��������Ϣ
	Kylin::DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(uFactorGId,dbItem))
		return NULL;
	
	PropertySet kFactorProp;
	kFactorProp.SetValue("$GID",uFactorGId);
	// ���ģ��
	DataItem::DataField dbField;
	if (dbItem.QueryField("MESH",dbField))
	{
		KSTR sMesh = boost::any_cast<KSTR>(dbField.m_aValue);	
		
		if (FileUtils::IsFileExist(sMesh))
		{
			OgreUtils::DynamicLoadMesh(sMesh);
			KSTR sName = FileUtils::GetFileNameWithSuffix(sMesh);
			kFactorProp.SetValue("$Mesh",sName);
		}	
	}
	// ��ò���
	if (dbItem.QueryField("MATERIAL",dbField))
	{
		KSTR sMat = boost::any_cast<KSTR>(dbField.m_aValue);	
		kFactorProp.SetValue("$Material",sMat);
	}

	// �������
	if (dbItem.QueryField("SCALE",dbField))
	{
		KFLOAT fScale = boost::any_cast<KFLOAT>(dbField.m_aValue);	
		kFactorProp.SetValue("$Scale",fScale);
	}

	// �������
	if (dbItem.QueryField("TYPE",dbField))
	{
		KSTR sClassid	= boost::any_cast<KSTR>(dbField.m_aValue);
		KUINT uClassid	= (KUINT)OgreRoot::GetSingletonPtr()->GetScriptVM()->GetGlobalNumber(sClassid.data());

		kFactorProp.SetValue("$CLASS_ID",uClassid);
	}
	//////////////////////////////////////////////////////////////////////////
	// ������Ч���ԣ�����״̬
	if (dbItem.QueryField("SPAWN_EFF",dbField))
	{
		KINT nEffectID = boost::any_cast<KINT>(dbField.m_aValue);	
		KANY aRet;
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"TEMPLATE",aRet) )
		{
			KSTR sEffect = boost::any_cast<KSTR>(aRet);	
			kFactorProp.SetValue("$SpawnEffect",sEffect);
		}
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"SCALE",aRet) )
		{
			KFLOAT fScale = boost::any_cast<KFLOAT>(aRet);	
			kFactorProp.SetValue("$SpawnEffectScale",fScale);
		}
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"TIMES",aRet) )
		{
			KFLOAT fScale = boost::any_cast<KFLOAT>(aRet);	
			kFactorProp.SetValue("$SpawnEffectTime",fScale);
		}
	}
	if (dbItem.QueryField("IDLE_EFF",dbField))
	{
		KINT nEffectID = boost::any_cast<KINT>(dbField.m_aValue);	
		KANY aRet;
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"TEMPLATE",aRet) )
		{
			KSTR sEffect = boost::any_cast<KSTR>(aRet);	
			kFactorProp.SetValue("$IdleEffect",sEffect);
		}
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"SCALE",aRet) )
		{
			KFLOAT fScale = boost::any_cast<KFLOAT>(aRet);	
			kFactorProp.SetValue("$IdleEffectScale",fScale);
		}
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"TIMES",aRet) )
		{
			KFLOAT fScale = boost::any_cast<KFLOAT>(aRet);	
			kFactorProp.SetValue("$IdleEffectTime",fScale);
		}
	}
	if (dbItem.QueryField("DESTROY_EFF",dbField))
	{
		KINT nEffectID = boost::any_cast<KINT>(dbField.m_aValue);	
		KANY aRet;
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"TEMPLATE",aRet) )
		{
			KSTR sEffect = boost::any_cast<KSTR>(aRet);	
			kFactorProp.SetValue("$DestroyEffect",sEffect);
		}
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"SCALE",aRet) )
		{
			KFLOAT fScale = boost::any_cast<KFLOAT>(aRet);	
			kFactorProp.SetValue("$DestroyEffectScale",fScale);
		}
		if ( DataManager::GetSingletonPtr()->Select("EFFECT_DB",nEffectID,"TIMES",aRet) )
		{
			KFLOAT fScale = boost::any_cast<KFLOAT>(aRet);	
			kFactorProp.SetValue("$DestroyEffectTime",fScale);
		}
	}

	return SpawnFactor(kFactorProp);
}
//-------------------------------------------------------------------
Kylin::Factor* Kylin::Action::SpawnFactor( PropertySet& kFactorProp )
{
	//////////////////////////////////////////////////////////////////////////
	// ��������ʵ��
	Factor* pFactor = BtStaticCast(Factor,KylinRoot::GetSingletonPtr()->SpawnEntity(kFactorProp));

	SAFE_CALL(pFactor,SetHostAction(this));

	if (m_pEmitterNode)
	{
		KPoint3 kInitPos = m_pEmitterNode->_getDerivedPosition();
		kInitPos.y = 2.0f;
		pFactor->SetTranslate(kInitPos);
	}

	m_kFactorList.push_back(pFactor->GetID());

	return pFactor;
}
//-------------------------------------------------------------------
KSTR Kylin::Action::GetIcon()
{
	KSTR sValue = "";
	m_kProperty.GetStrValue("$Icon",sValue);

	return sValue;
}
//-------------------------------------------------------------------
KSTR Kylin::Action::GetExplain()
{
	KSTR sValue = "";
	//m_kProperty.GetStrValue("$Icon",sValue);

	return sValue;
}

KVOID Kylin::Action::SetEmitterNode( Ogre::Node* pNode )
{
	m_pEmitterNode = pNode;
}

KUINT Kylin::Action::GetHostWorldID()
{
	return m_pDispatcher->GetHostWorldID();
}

ActionType Kylin::Action::TransformType( KCSTR& sType )
{
	return (ActionType)(KUINT)OgreRoot::GetSingletonPtr()->GetScriptVM()->GetGlobalNumber(sType.data());
}
//-------------------------------------------------------------------