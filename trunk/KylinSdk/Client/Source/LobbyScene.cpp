#include "cltpch.h"
#include "LobbyScene.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "XmlStream.h"
#include "uiLobbyMenu.h"

#include "SceneLoader.h"
#include "GameFrame.h"

#include "GameStatus.h"

#define SC_SHOWING	"Data\\script\\showing.xml"
#define LOBBY_SCENE	"lobby_scene.xml"

Kylin::LobbyScene::LobbyScene()
: m_uSelectedIndex(-1)
, m_pNode(NULL)
, m_fScale(1.0f)
, m_pCamera(NULL)
, m_pShowgirl(NULL)
, m_pSceneLoader(NULL)
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
	
	SpawnScene();
	LoadActors();	

	return true;
}

KBOOL Kylin::LobbyScene::LoadActors()
{
	XmlStream kXml(SC_SHOWING);
	if (!kXml.Open(XmlStream::Read))
		return false;
	
	kXml.SetToFirstChild("stage");

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
	// 设置摄像机朝向
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
	
 	// 加载角色信息
 	KBOOL bRes = kXml.SetToFirstChild("actor");
 	if (bRes)
 	{
		// 获得缩放参数
		m_fScale = kXml.GetAttrFloat("scale");

 		// 加载展示动画信息
 		KBOOL bAnim = kXml.SetToFirstChild("animation");
 		while(bAnim)
 		{
 			AnimUnit kAnim;
 			kAnim.mAnim = kXml.GetAttrString("name");
 			kAnim.mLoopFlag = kXml.GetAttrBool("loop");
 			if (kXml.HasAttr("time"))
 				kAnim.mTime = kXml.GetAttrFloat("time");
 			m_kAnimQueue.push_back(kAnim);
 			
 			bAnim = kXml.SetToNextChild("animation");
 		}

		// 设置位置
		kXml.SetToFirstChild("position");
		KSTR sPos = kXml.GetString("");
		m_kPosition = Ogre::StringConverter::parseVector3(sPos);
	
		kXml.SetToParent();

		// 设置朝向
		kXml.SetToFirstChild("orientation");
		sPos = kXml.GetString("");
		m_kQuaternion = Ogre::StringConverter::parseQuaternion(sPos);
		
		kXml.SetToParent();
 	}

	kXml.Close();
	
	//////////////////////////////////////////////////////////////////////////
	// test code
// 	PropertySet kProp;
// 	kProp.SetValue("$PlayerID",KUINT(1));
// 	KylinRoot::GetSingletonPtr()->GetCurrentGameStatus()->Serialize(kProp);

	//OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase("LobbyMenu")->SetWidgetEnable("_Main",true);
	//////////////////////////////////////////////////////////////////////////
	return true;
}

KVOID Kylin::LobbyScene::Tick( KFLOAT fElapsed )
{
	SAFE_CALL(m_pShowgirl,Tick(fElapsed));
}

KVOID Kylin::LobbyScene::Destroy()
{
	SAFE_DEL(m_pShowgirl);
	//////////////////////////////////////////////////////////////////////////
	SAFE_CALL(m_pSceneLoader,Unload(NULL));
	SAFE_DEL(m_pSceneLoader);
}

KVOID Kylin::LobbyScene::SpawnActor(KUINT uIndex)
{
	SAFE_DEL(m_pShowgirl);
	//////////////////////////////////////////////////////////////////////////
	m_pShowgirl = KNEW Showgirl(uIndex);
	if (!m_pShowgirl->Initialize())
		return;
	
	m_pShowgirl->SetScale(m_fScale);
	m_pShowgirl->SetTranslate(m_kPosition);
	//-----------------------------------------------------
	// 设置进入游戏按钮为可用状态
	LobbyMenu* pMenu = GET_GUI_PTR(LobbyMenu);
	SAFE_CALL(pMenu,SetWidgetEnable("_Main",true));
	//-----------------------------------------------------
}

KVOID Kylin::LobbyScene::SpawnScene()
{
	m_pSceneLoader = KylinRoot::GetSingletonPtr()->GetGameFramePtr()->CreateSceneLoader();

	if (!m_pSceneLoader->LoadScene(LOBBY_SCENE))
	{
		assert(!"场景加载失败！");
	}
}