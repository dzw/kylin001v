#include "engpch.h"
#include "Node.h"
#include "rOgreRoot.h"


Kylin::Node::Node()
: m_pOgreNode(NULL)
, m_pOgreEntity(NULL)
{

}

Kylin::Node::~Node()
{
	if (m_pOgreEntity)
		OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyEntity(m_pOgreEntity);
	if (m_pOgreNode)
		OgreRoot::GetSingletonPtr()->GetSceneManager()->destroySceneNode(m_pOgreNode);
}

KBOOL Kylin::Node::Load( Kylin::PropertySet kProp )
{
	KSTR sMesh, sMaterials;
	if (kProp.GetStrValue("$MESH",sMesh))
	{
		kProp.GetStrValue("$MATERIALS",sMaterials);
		// 加载模型资源
		if (!Ogre::MeshManager::getSingletonPtr()->resourceExists("sphere.mesh"))
		{
			Ogre::MeshPtr pMesh = Ogre::MeshManager::getSingleton().load(sMesh, "General");
			//if (pMesh == NULL) 
			//	return false;
		}
		
		m_pOgreNode		= OgreRoot::GetSingletonPtr()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
		m_pOgreEntity	= OgreRoot::GetSingletonPtr()->GetSceneManager()->createEntity(sMesh);
		
		// 设置模型贴图
		if (!sMaterials.empty())
			m_pOgreEntity->setMaterialName(sMaterials);
		
		if (!m_pOgreNode || !m_pOgreEntity) return false;
		// 挂接对象
		m_pOgreNode->attachObject(m_pOgreEntity);

		// 设置影子
		KBOOL bShadows = false;
		if (kProp.GetBoolValue("$SHADOWS",bShadows) && bShadows)
			m_pOgreEntity->setCastShadows(bShadows);
		else
			m_pOgreEntity->setCastShadows(false);

		// 设置渲染距离
		KFLOAT fRenderDisance = -1.0f;
		if (kProp.GetFloatValue("$RENDER_DISTANCE",fRenderDisance) && fRenderDisance > 0)
			m_pOgreEntity->setRenderingDistance(fRenderDisance);
	}
	
	KBOOL bCollide = false;
	if (kProp.GetBoolValue("$COLLISION",bCollide))
	{
		if (!bCollide || !SetupCllsn(m_pOgreEntity,kProp))
		{
			return false;
		}
	}

	return true;
}

KVOID Kylin::Node::Unload()
{

}

KVOID Kylin::Node::Tick( KFLOAT fElapsed )
{

}

KVOID Kylin::Node::SetTranslate( KPoint3 kPos )
{
	m_pOgreNode->setPosition(kPos);
}

KPoint3 Kylin::Node::GetTranslate()
{
	return m_pOgreNode->getPosition();
}

KVOID Kylin::Node::SetVisible( KBOOL bFlag )
{
	m_pOgreEntity->setVisible(bFlag);
}

KBOOL Kylin::Node::GetVisible()
{
	return m_pOgreEntity->isVisible();
}

KVOID Kylin::Node::SetScale( KFLOAT fScale )
{
	m_pOgreNode->setScale(KPoint3(fScale,fScale,fScale));
}

KFLOAT Kylin::Node::GetScale()
{
	return m_pOgreNode->getScale().x;
}

Ogre::SceneNode* Kylin::Node::GetSceneNode()
{
	return m_pOgreNode;
}

KVOID Kylin::Node::SetYaw( KFLOAT fYaw )
{
	Quaternion q;
	q.FromAngleAxis(Ogre::Radian(Ogre::Degree(Ogre::Real(fYaw))),Vector3::UNIT_Y);

	m_pOgreNode->setOrientation(q);
}