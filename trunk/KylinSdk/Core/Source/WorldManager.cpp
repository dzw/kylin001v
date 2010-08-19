#include "corepch.h"
#include "WorldManager.h"
#include "Scene.h"
#include "XmlStream.h"

#define CFG_WORLD "Data/world/World.xml"

Kylin::WorldManager::WorldManager()
: m_pActiveScene(NULL)
{
	
}

Kylin::WorldManager::~WorldManager()
{

}


KBOOL Kylin::WorldManager::Initialize( KCCHAR* pWorldCfg )
{
	XmlStream kXml(CFG_WORLD);
	if (!kXml.Open(XmlStream::Read))
		return false;
	
	//////////////////////////////////////////////////////////////////////////
	// 读取入口信息
	kXml.SetToFirstChild("Entry");
	KUINT uEntry = kXml.GetInt(-1);
	if (uEntry < 0 ) return false;
	kXml.SetToParent();

	//////////////////////////////////////////////////////////////////////////
	// 加载所有场景配置
	KBOOL bScene = kXml.SetToFirstChild("Scene");
	while (bScene)
	{
		SceneHag* hag = KNEW SceneHag;
		
		hag->m_uSceneID		= kXml.GetAttrInt("id");
		hag->m_sName		= kXml.GetAttrString("name");
		hag->m_uType		= kXml.GetAttrInt("type");

		kXml.SetToFirstChild("File");
		hag->m_sSceneFile	= kXml.GetString("");

		kXml.SetToParent();
		kXml.SetToFirstChild("Level");
		hag->m_uLevel		= kXml.GetInt(0);

		kXml.SetToParent();
		kXml.SetToFirstChild("BackgroundSound");
		hag->m_nBgSound		= kXml.GetInt(-1);

		kXml.SetToParent();

		hag->m_bPass = false;

		m_kSceneInfo.insert(std::pair<KUINT,SceneHag*>(hag->m_uSceneID,hag));

		bScene = kXml.SetToNextChild("Scene");
	}

	kXml.Close();
	//////////////////////////////////////////////////////////////////////////
	// 加载场景
	EnterScene(uEntry);

	return true;
}

KVOID Kylin::WorldManager::Destroy()
{
	KMAP<KUINT,SceneHag*>::iterator begin = m_kSceneInfo.begin();
	KMAP<KUINT,SceneHag*>::iterator end   = m_kSceneInfo.end();
	for (KMAP<KUINT,SceneHag*>::iterator it=begin; it != end; it++)
	{
		KDEL it->second;
	}
	m_kSceneInfo.clear();

	m_pActiveScene->LeaveScene();
	SAFE_DEL(m_pActiveScene);
}

KBOOL Kylin::WorldManager::EnterScene( KINT nSceneID )
{
	KMAP<KUINT,SceneHag*>::iterator it = m_kSceneInfo.find(nSceneID);
	if (it != m_kSceneInfo.end())
	{
		m_pActiveScene = KNEW Scene(*(it->second));
		m_pActiveScene->EnterScene();
	}
	
	return true;
}

KVOID Kylin::WorldManager::SwitchScene( KINT idTargetScene )
{
	m_pActiveScene->LeaveScene();
	SAFE_DEL(m_pActiveScene);
	
	EnterScene(idTargetScene);
}

Kylin::Scene* Kylin::WorldManager::GetActiveScene( KVOID )
{
	return m_pActiveScene;
}

KVOID Kylin::WorldManager::Tick( KFLOAT fElapsed )
{
	SAFE_CALL(m_pActiveScene,Tick(fElapsed));
}
