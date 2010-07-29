#include "cltpch.h"
#include "Showgirl.h"
#include "DataItem.h"
#include "DataManager.h"
#include "FileUtils.h"
#include "rOgreUtils.h"
#include "AnimationProxy.h"
#include "KylinRoot.h"
#include "GameStatus.h"
#include "DataLoader.h"


Kylin::Showgirl::Showgirl(KUINT uGid)
: m_uGid(uGid)
, m_uCurAnimIndex(-1)
, m_fTempTime(0)
, m_eShowFlag(SF_NODE_)
{

}

KBOOL Kylin::Showgirl::Initialize(AnimQueue que)
{
	KSTR sValue;
	if (!DataManager::GetSingletonPtr()->GetGlobalValue("CHAR_DB",sValue))
		return false;

	Kylin::DataLoader* pLoader = DataManager::GetSingletonPtr()->GetLoaderPtr(sValue);
	// 查询对应的角色信息
	Kylin::DataItem dbItem;
	if (!pLoader->GetDBPtr()->Query(m_uGid,dbItem))
		return false;
	
	PropertySet kSet;
	DataItem::DataField dbField;
	dbItem.QueryField("MESH",dbField);
	KSTR sModel = boost::any_cast<KSTR>(dbField.m_aValue);

	// 注： 路径前不可有 "\"
	if (!FileUtils::IsFileExist(sModel))
		return false;

	OgreUtils::DynamicLoadMesh(sModel);
 	KSTR sName = FileUtils::GetFileNameWithSuffix(sModel);

	//////////////////////////////////////////////////////////////////////////
	kSet.SetValue("$Mesh",sName);

	if (!Load(kSet))
		return false;
	
	//-----------------------------------------------------
	Spawn();
	//-----------------------------------------------------
	m_kAnimQueue = que;

	return true;
}

KVOID Kylin::Showgirl::Tick( KFLOAT fElapsed )
{
	Node::Tick(fElapsed);

	if (m_eShowFlag == SF_PREPARE_)
		OnPrepare(fElapsed);
	else if (m_eShowFlag == SF_SHOWING_)
		OnShowing(fElapsed);
}

KVOID Kylin::Showgirl::Spawn()
{
	m_eShowFlag = SF_PREPARE_;

	//-----------------------------------------------------
	// 设置玩家GID
	PropertySet kProp;
	kProp.SetValue("$PlayerID",m_uGid);
	KylinRoot::GetSingletonPtr()->GetCurrentGameStatus()->Serialize(kProp);
	//-----------------------------------------------------
}

KVOID Kylin::Showgirl::OnPrepare(KFLOAT fElapsed)
{
	m_eShowFlag = SF_SHOWING_;
	m_uCurAnimIndex = 0;
	m_fTempTime = .0f;
	
	if (m_uCurAnimIndex < m_kAnimQueue.size())
		GetAnimationProxy()->Play(m_kAnimQueue[m_uCurAnimIndex].mAnim,m_kAnimQueue[m_uCurAnimIndex].mLoopFlag);
}

KVOID Kylin::Showgirl::OnShowing(KFLOAT fElapsed)
{
	if (m_uCurAnimIndex >= m_kAnimQueue.size())
		return;

	if (m_kAnimQueue[m_uCurAnimIndex].mLoopFlag)
	{
		if (m_fTempTime < m_kAnimQueue[m_uCurAnimIndex].mTime)
		{
			m_fTempTime += fElapsed;
			if (m_fTempTime >= m_kAnimQueue[m_uCurAnimIndex].mTime)
			{
				m_fTempTime = .0f;
				m_uCurAnimIndex++;
				if (m_uCurAnimIndex >= m_kAnimQueue.size())
					m_uCurAnimIndex = 0;
				GetAnimationProxy()->Play(m_kAnimQueue[m_uCurAnimIndex].mAnim,m_kAnimQueue[m_uCurAnimIndex].mLoopFlag);
			}
		}
	}
	else if (GetAnimationProxy()->GetLength(m_kAnimQueue[m_uCurAnimIndex].mAnim) > m_fTempTime)
	{
		m_fTempTime += fElapsed;
		if (GetAnimationProxy()->GetLength(m_kAnimQueue[m_uCurAnimIndex].mAnim) <= m_fTempTime)
		{
			m_fTempTime = .0f;
			m_uCurAnimIndex++;
			if (m_uCurAnimIndex >= m_kAnimQueue.size())
				m_uCurAnimIndex = 0;
			GetAnimationProxy()->Play(m_kAnimQueue[m_uCurAnimIndex].mAnim,m_kAnimQueue[m_uCurAnimIndex].mLoopFlag);
		}
	}
}