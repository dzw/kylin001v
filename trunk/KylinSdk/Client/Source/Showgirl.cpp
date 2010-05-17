#include "cltpch.h"
#include "Showgirl.h"
#include "DataItem.h"
#include "DataManager.h"
#include "FileUtils.h"
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

KBOOL Kylin::Showgirl::Initialize()
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
	dbItem.QueryField("MATERIAL",dbField);
	KSTR sMaterials = boost::any_cast<KSTR>(dbField.m_aValue);
	// 注： 路径前不可有 "\"
	if (!FileUtils::IsFileExist(sModel))
		return false;

	KSTR sName = FileUtils::GetFileNameWithSuffix(sModel);
	KSTR sDir  = FileUtils::GetRelativePath(sModel);
	if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceExists("General",sName))
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(sDir,"FileSystem","General");
	
	//////////////////////////////////////////////////////////////////////////
	// addResourceLocation 此方法材质脚步无法解析需要手动加载
 	KSTR sMat = FileUtils::GetFileName(sModel);
 	sMat += ".material";
 	if (!Ogre::MaterialManager::getSingletonPtr()->resourceExists(sMat))
	{
		sMat = sDir + sMat;
		std::ifstream fs(sMat.data());
		Ogre::DataStreamPtr stream = Ogre::DataStreamPtr(OGRE_NEW Ogre::FileStreamDataStream(&fs, false));
		Ogre::MaterialManager::getSingletonPtr()->parseScript(stream,"General");
	}
	//////////////////////////////////////////////////////////////////////////

	kSet.SetValue("$Mesh",sName);
	kSet.SetValue("$Materials",sMaterials);

	if (!Load(kSet))
		return false;
	
	m_pOgreEntity->setQueryFlags(QUERYFLAG_SHOWGIRL);

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
	//GetAnimationProxy()->Play(m_sIdleAnim,true);
}

KVOID Kylin::Showgirl::Showing( KBOOL bFlag )
{
	if (bFlag)
	{
		m_eShowFlag = SF_PREPARE_;

		PropertySet kProp;
		kProp.SetValue("$PlayerID",m_uGid);
		KylinRoot::GetSingletonPtr()->GetCurrentGameStatus()->Serialize(kProp);
	}
	else
	{
		m_eShowFlag = SF_NODE_;
		GetAnimationProxy()->Play(m_sIdleAnim);
	}
}

KVOID Kylin::Showgirl::OnPrepare(KFLOAT fElapsed)
{
	m_eShowFlag = SF_SHOWING_;
	m_uCurAnimIndex = 0;
	m_fTempTime = .0f;

	GetAnimationProxy()->Play(m_kAnimQueue[m_uCurAnimIndex].mAnim,m_kAnimQueue[m_uCurAnimIndex].mLoopFlag);
}

KVOID Kylin::Showgirl::OnShowing(KFLOAT fElapsed)
{
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