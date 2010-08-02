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

	// ���������λ��
	kXml.SetToFirstChild("camera_pos");
	KSTR sPos = kXml.GetString("");
	Ogre::vector<Ogre::String>::type kV = Ogre::StringUtil::split(sPos," ");
	KPoint3 kPos;
	kPos.x = atof(kV[0].data());
	kPos.y = atof(kV[1].data());
	kPos.z = atof(kV[2].data());
	m_pCamera->setPosition(kPos);
	
	kXml.SetToParent();
	// �������������
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
	
 	// ���ؽ�ɫ��Ϣ
 	KBOOL bRes = kXml.SetToFirstChild("actor");
 	if (bRes)
 	{
		// ������Ų���
		m_fScale = kXml.GetAttrFloat("scale");

 		// ����չʾ������Ϣ
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

		// ����λ��
		kXml.SetToFirstChild("position");
		KSTR sPos = kXml.GetString("");
		m_kPosition = Ogre::StringConverter::parseVector3(sPos);
	
		kXml.SetToParent();

		// ���ó���
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

#ifdef _DEBUG
bool bsd = true;
#endif
KVOID Kylin::LobbyScene::Tick( KFLOAT fElapsed )
{
	SAFE_CALL(m_pShowgirl,Tick(fElapsed));
	SAFE_CALL(m_pSceneLoader,Tick(fElapsed));

	//-------------------------
	// test code
#ifdef _DEBUG
	if (bsd)
	{
		SpawnActor(1);
		bsd = false;
	}
#endif
}

KVOID Kylin::LobbyScene::Destroy()
{
	SAFE_CALL(m_pShowgirl,Destroy());
	SAFE_DEL(m_pShowgirl);
	//////////////////////////////////////////////////////////////////////////
	SAFE_CALL(m_pSceneLoader,Unload(NULL));
	SAFE_DEL(m_pSceneLoader);
}

KVOID Kylin::LobbyScene::SpawnActor(KUINT uIndex)
{
	SAFE_CALL(m_pShowgirl,Destroy());
	SAFE_DEL(m_pShowgirl);
	//////////////////////////////////////////////////////////////////////////
	m_pShowgirl = KNEW Showgirl(uIndex);
	if (!m_pShowgirl->Initialize(m_kAnimQueue))
		return;
	
	m_pShowgirl->SetScale(m_fScale);
	m_pShowgirl->SetTranslate(m_kPosition);
	//-----------------------------------------------------
	// ���ý�����Ϸ��ťΪ����״̬
	LobbyMenu* pMenu = GET_GUI_PTR(LobbyMenu);
	SAFE_CALL(pMenu,SetWidgetEnable("_Main",true));
	//-----------------------------------------------------
}

KVOID Kylin::LobbyScene::SpawnScene()
{
	m_pSceneLoader = KylinRoot::GetSingletonPtr()->GetGameFramePtr()->CreateSceneLoader();

	if (!m_pSceneLoader->LoadScene(LOBBY_SCENE))
	{
		assert(!"��������ʧ�ܣ�");
	}
}