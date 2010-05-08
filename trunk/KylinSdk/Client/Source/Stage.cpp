#include "cltpch.h"
#include "Stage.h"
#include "Showgirl.h"
#include "rOgreRoot.h"
#include "CameraControl.h"
#include "CCSCameraControlSystem.h"
#include "XmlStream.h"


#define SC_SHOWING	"Data\\script\\showing.xml"

Kylin::Stage::Stage()
: m_eShowFlag(Stage::SF_NODE_)
, m_pNode(NULL)
, m_pLight(NULL)
{

}

Kylin::Stage::~Stage()
{
	Destroy();
}

KBOOL Kylin::Stage::Initialize()
{
	Ogre::Camera* pCam = OgreRoot::GetSingletonPtr()->CreateCamera("$MainCamera");
	if (pCam)
	{
		pCam->setNearClipDistance(0.2f);
		OgreRoot::GetSingletonPtr()->CreateCameraControl(pCam);
	}

	OgreRoot::GetSingletonPtr()->GetMainWindow()->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);

	//////////////////////////////////////////////////////////////////////////	
	m_pLight = OgreRoot::GetSingletonPtr()->GetSceneManager()->createLight("LobbyLight");
	m_pLight->setType(Ogre::Light::LT_DIRECTIONAL);
	Ogre::Vector3 vec(-0.3, -0.3, -0.3);
	vec.normalise();

	m_pNode = OgreRoot::GetSingletonPtr()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();

	Ogre::MeshManager::getSingleton().createPlane(
		"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 1000, 1000, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* pEntity = OgreRoot::GetSingletonPtr()->GetSceneManager()->createEntity("FloorPlane", "FloorPlane");
	pEntity->setMaterialName("Ground");
	m_pNode->attachObject(pEntity);
	
	OgreRoot::GetSingletonPtr()->GetCameraController()->SetTarget(m_pNode);
	OgreRoot::GetSingletonPtr()->GetCameraController()->SetMode("FirstPerson");
	//////////////////////////////////////////////////////////////////////////	
	
	LoadActors();	

	return true;
}

KBOOL Kylin::Stage::LoadActors()
{
	XmlStream kXml(SC_SHOWING);
	if (!kXml.Open(XmlStream::Read))
		return false;
	
	kXml.SetToFirstChild("stage");
	// 加载舞台信息
	kXml.SetToParent();
	
	// 加载角色信息
	KBOOL bRes = kXml.SetToFirstChild("actor");
	while (bRes)
	{
		KINT nGid = kXml.GetAttrInt("gid");
		
		Showgirl* pGirl = KNEW Showgirl(nGid);
		
		kXml.SetToFirstChild("idle");
		pGirl->m_sIdleAnim = kXml.GetAttrString("name");
		kXml.SetToParent();
		// 加载展示动画信息
		KBOOL bAnim = kXml.SetToFirstChild("animation");
		while(bAnim)
		{
			AnimUnit kAnim;
			kAnim.mAnim = kXml.GetAttrString("name");
			kAnim.mLoopFlag = kXml.GetAttrBool("loop");
			if (kXml.HasAttr("time"))
				kAnim.mTime = kXml.GetAttrFloat("time");
			pGirl->m_kAnimQueue.push_back(kAnim);
			
			bAnim = kXml.SetToNextChild("animation");
		}
		// 产生showgirl
		if (pGirl->Initialize())
		{
			// 设置位置
			kXml.SetToFirstChild("position");
			KSTR sPos = kXml.GetString("");
			Ogre::vector<Ogre::String>::type kV = Ogre::StringUtil::split(sPos," ");
			KPoint3 kPos;
			kPos.x = atof(kV[0].data());
			kPos.y = atof(kV[1].data());
			kPos.z = atof(kV[2].data());
			pGirl->SetTranslate(kPos);

			kXml.SetToParent();

			m_kActorVec.push_back(pGirl);
		}
		
		bRes = kXml.SetToNextChild("actor");
	}

	kXml.Close();

	return true;
}

KVOID Kylin::Stage::Tick( KFLOAT fElapsed )
{

}

KVOID Kylin::Stage::Destroy()
{
	for (KUINT i = 0; i < m_kActorVec.size(); i++)
	{
		KDEL m_kActorVec[i];
	}

	m_kActorVec.clear();
}

KVOID Kylin::Stage::OnPrepare( KFLOAT fElapsed )
{

}

KVOID Kylin::Stage::OnShowing( KFLOAT fElapsed )
{

}

Kylin::Showgirl* Kylin::Stage::SpawnActor( const KUINT& uGid )
{

	return NULL;
}