#include "cltpch.h"
#include "LobbyScene.h"
#include "Showgirl.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "XmlStream.h"

#include "GuiManager.h"
#include "SceneLoader.h"
#include "GameFrame.h"

#include "GameStatus.h"

#define SC_SHOWING	"Data\\script\\showing.xml"
#define LOBBY_SCENE	"lobby_scene.xml"

Kylin::LobbyScene::LobbyScene()
: m_uSelectedIndex(-1)
, m_pNode(NULL)
{

}

Kylin::LobbyScene::~LobbyScene()
{
	Destroy();
}

KBOOL Kylin::LobbyScene::Initialize()
{
	m_pCamera = OgreRoot::GetSingletonPtr()->CreateCamera("$MainCamera");
	if (m_pCamera)
	{
		m_pCamera->setNearClipDistance(0.2f);
		
		OgreRoot::GetSingletonPtr()->CreateViewports(m_pCamera,Ogre::ColourValue::Black);
	}
	//////////////////////////////////////////////////////////////////////////	
	
//	m_pNode = OgreRoot::GetSingletonPtr()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
	
	SpawnScene();
	LoadActors();	
//	SpawnActors();

	return true;
}

KBOOL Kylin::LobbyScene::LoadActors()
{
	XmlStream kXml(SC_SHOWING);
	if (!kXml.Open(XmlStream::Read))
		return false;
	
	kXml.SetToFirstChild("stage");
// 	// 加载舞台信息
// 	kXml.SetToFirstChild("skybox");
// 	KSTR sSky = kXml.GetString("");
// 	
// 	kXml.SetToParent();
	// 设置摄像机位置
	kXml.SetToFirstChild("camera_pos");
	KSTR sPos = kXml.GetString("");
	Ogre::vector<Ogre::String>::type kV = Ogre::StringUtil::split(sPos," ");
	KPoint3 kPos;
	kPos.x = atof(kV[0].data());
	kPos.y = atof(kV[1].data());
	kPos.z = atof(kV[2].data());
	m_pCamera->setPosition(kPos);
	
	kXml.SetToParent();
	// 设置观察位置
	kXml.SetToFirstChild("camera_ori");
	sPos = kXml.GetString("");
	kV = Ogre::StringUtil::split(sPos," ");
	Ogre::Quaternion kOrg;
	kOrg.w = atof(kV[0].data());
	kOrg.x = atof(kV[1].data());
	kOrg.y = atof(kV[2].data());
	kOrg.z = atof(kV[3].data());
	m_pCamera->setOrientation(kOrg);

	kXml.SetToParent();
	kXml.SetToParent();
	
// 	// 加载角色信息
// 	KBOOL bRes = kXml.SetToFirstChild("actor");
// 	while (bRes)
// 	{
// 		KINT nGid = kXml.GetAttrInt("gid");
// 		
// 		Showgirl* pGirl = KNEW Showgirl(nGid);
// 		
// 		kXml.SetToFirstChild("idle");
// 		pGirl->m_sIdleAnim = kXml.GetAttrString("name");
// 		kXml.SetToParent();
// 		// 加载展示动画信息
// 		KBOOL bAnim = kXml.SetToFirstChild("animation");
// 		while(bAnim)
// 		{
// 			AnimUnit kAnim;
// 			kAnim.mAnim = kXml.GetAttrString("name");
// 			kAnim.mLoopFlag = kXml.GetAttrBool("loop");
// 			if (kXml.HasAttr("time"))
// 				kAnim.mTime = kXml.GetAttrFloat("time");
// 			pGirl->m_kAnimQueue.push_back(kAnim);
// 			
// 			bAnim = kXml.SetToNextChild("animation");
// 		}
// 		// 产生showgirl
// 		if (pGirl->Initialize())
// 		{
// 			// 设置位置
// 			kXml.SetToFirstChild("position");
// 			KSTR sPos = kXml.GetString("");
// 			Ogre::vector<Ogre::String>::type kV = Ogre::StringUtil::split(sPos," ");
// 			KPoint3 kPos;
// 			kPos.x = atof(kV[0].data());
// 			kPos.y = atof(kV[1].data());
// 			kPos.z = atof(kV[2].data());
// 			pGirl->SetTranslate(kPos);
// 
// 			kXml.SetToParent();
// 
// 			m_kActorVec.push_back(pGirl);
// 		}
// 		
// 		bRes = kXml.SetToNextChild("actor");
// 	}

	kXml.Close();
	
// 	if (!sSky.empty())
// 		OgreRoot::GetSingletonPtr()->GetSceneManager()->setSkyDome(true,sSky,22,1);

	//////////////////////////////////////////////////////////////////////////
	// test code
	PropertySet kProp;
	kProp.SetValue("$PlayerID",KUINT(1));
	KylinRoot::GetSingletonPtr()->GetCurrentGameStatus()->Serialize(kProp);

	//OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase("LobbyMenu")->SetWidgetEnable("_Main",true);
	//////////////////////////////////////////////////////////////////////////
	return true;
}

KVOID Kylin::LobbyScene::Tick( KFLOAT fElapsed )
{
	for (KUINT i = 0; i < m_kActorVec.size(); i++)
	{
		m_kActorVec[i]->Tick(fElapsed);
	}
}

KVOID Kylin::LobbyScene::Destroy()
{
	for (KUINT i = 0; i < m_kActorVec.size(); i++)
	{
		KDEL m_kActorVec[i];
	}

	m_kActorVec.clear();
	//////////////////////////////////////////////////////////////////////////
	SAFE_CALL(m_pSceneLoader,Unload(NULL));
	SAFE_DEL(m_pSceneLoader);
}

KVOID Kylin::LobbyScene::SpawnActors()
{
	for (KUINT i = 0; i < m_kActorVec.size(); i++)
	{
		m_kActorVec[i]->Spawn();
	}
}

KVOID Kylin::LobbyScene::SpawnScene()
{
	m_pSceneLoader = KylinRoot::GetSingletonPtr()->GetGameFramePtr()->CreateSceneLoader();

	if (!m_pSceneLoader->LoadScene(LOBBY_SCENE))
	{
		assert(!"场景加载失败！");
	}
}