#include "engpch.h"
#include "Node.h"
#include "rOgreRoot.h"
#include "rOgreUtils.h"
#include "AnimationProxy.h"
#include "EffectManager.h"
#include "EntityMaterialInstance.h"


Kylin::Node::Node()
: m_pOgreNode(NULL)
, m_pOgreEntity(NULL)
, m_pAnimProxy(NULL)
, m_pTransparency(NULL)
{
	m_pAnimProxy = KNEW AnimationProxy();
}

Kylin::Node::~Node()
{
	Destroy();
}

KBOOL Kylin::Node::Load( Kylin::PropertySet kProp )
{
	KUINT uID = -1;
	if (kProp.GetUIntValue("$ID",uID))
		SetWorldID(uID);
	// 
	m_pOgreNode	= OgreRoot::GetSingletonPtr()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();

	//////////////////////////////////////////////////////////////////////////
	KSTR sMesh, sMaterials;
	if (kProp.GetStrValue("$Mesh",sMesh) && !sMesh.empty())
	{
		//kProp.GetStrValue("$Materials",sMaterials);
		// 加载模型资源
		if (uID != -1)
			m_pOgreEntity	= OgreRoot::GetSingletonPtr()->GetSceneManager()->createEntity(Ogre::StringConverter::toString(uID),sMesh,"General");
		else
			m_pOgreEntity	= OgreRoot::GetSingletonPtr()->GetSceneManager()->createEntity(sMesh);

		// 设置模型贴图
		OgreUtils::SetDefaultMaterial(m_pOgreEntity);
		//if (!sMaterials.empty())
		//	m_pOgreEntity->setMaterialName(sMaterials);
		
		if (!m_pOgreNode || !m_pOgreEntity) return false;
		// 挂接对象
		m_pOgreNode->attachObject(m_pOgreEntity);

		// 设置影子
		KBOOL bShadows = false;
		if (kProp.GetBoolValue("$Shadows",bShadows) && bShadows)
			m_pOgreEntity->setCastShadows(bShadows);
		else
			m_pOgreEntity->setCastShadows(false);
		
		//--------------------------------------------------------
		// 透明度
		if (!m_pTransparency)
			m_pTransparency = KNEW EntityMaterialInstance (m_pOgreEntity);
		//--------------------------------------------------------

		// 设置渲染距离
		KFLOAT fRenderDisance = -1.0f;
		if (kProp.GetFloatValue("$RENDER_DISTANCE",fRenderDisance) && fRenderDisance > 0)
			m_pOgreEntity->setRenderingDistance(fRenderDisance);
		
		// 设置动画
		m_pAnimProxy->SetTarget(m_pOgreEntity);
	}
	//////////////////////////////////////////////////////////////////////////
	// 设置缩放参数
	KFLOAT fScale;
	if (kProp.GetFloatValue("$Scale",fScale))
		m_pOgreNode->setScale(KPoint3(fScale,fScale,fScale));
	//////////////////////////////////////////////////////////////////////////
	// 加载特效
	KSTR sEffect;
	if (kProp.GetStrValue("$Effect",sEffect))
	{
		EffectObject* pObj = AttachEffect(sEffect);
		if (pObj)
		{	// 设置特效缩放
			KFLOAT fEfScale;
			if (kProp.GetFloatValue("$EffectScale",fEfScale))
			{
				pObj->SetScale(fEfScale);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 加载碰撞
	KBOOL bCollide = false;
	if (kProp.GetBoolValue("$Collision",bCollide))
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
	if (m_pAnimProxy) m_pAnimProxy->Update(fElapsed);
}

KVOID Kylin::Node::SetTranslate( KPoint3 kPos )
{
	if (m_pOgreNode)
		m_pOgreNode->setPosition(kPos);
}

KPoint3 Kylin::Node::GetTranslate()
{
	return m_pOgreNode->getPosition();
}

KVOID Kylin::Node::SetVisible( KBOOL bFlag )
{
	if (m_pOgreEntity)
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

Kylin::AnimationProxy* Kylin::Node::GetAnimationProxy()
{
	return m_pAnimProxy;
}

KVOID Kylin::Node::Destroy()
{
	//--------------------------------------------------------
	SAFE_DEL(m_pTransparency);
	//--------------------------------------------------------
	for (KUINT i = 0; i < m_kEffectList.size(); i++)
		EffectManager::GetSingletonPtr()->DestroyEffect(m_kEffectList[i]->GetName());
	m_kEffectList.clear();

	//--------------------------------------------------------
	SAFE_DEL(m_pAnimProxy);

	SAFE_CALL(m_pOgreNode,removeAndDestroyAllChildren());
	//--------------------------------------------------------
	if (m_pOgreEntity && OgreRoot::GetSingletonPtr()->GetSceneManager()->hasEntity(m_pOgreEntity->getName()))
		OgreRoot::GetSingletonPtr()->GetSceneManager()->destroyEntity(m_pOgreEntity);
	if (m_pOgreNode && OgreRoot::GetSingletonPtr()->GetSceneManager()->hasSceneNode(m_pOgreNode->getName()))
		OgreRoot::GetSingletonPtr()->GetSceneManager()->destroySceneNode(m_pOgreNode);
}

Ogre::Entity* Kylin::Node::GetEntityPtr()
{
	return m_pOgreEntity;
}

Kylin::EffectObject* Kylin::Node::AttachEffect( KSTR sName, KUINT uType /*= EffectManager::ET_PARTICLE*/, KPoint3 kPos )
{
	KSTR sNewName = sName + Ogre::StringConverter::toString(GetWorldID());
	EffectObject* pObj = EffectManager::GetSingletonPtr()->Generate(sNewName,sName,uType);
	if (pObj)
	{
		pObj->Attach(m_pOgreNode,kPos);
		
		m_kEffectList.push_back(pObj);
	}

	return pObj;
}

KVOID Kylin::Node::DetachAndDestroyEffect( KSTR sName )
{
	KSTR sNewName = sName + GetEntityName();
	for (KUINT i = 0; i < m_kEffectList.size(); i++)
	{	
		if (m_kEffectList[i]->GetName() == sNewName)
		{
			EffectManager::GetSingletonPtr()->DestroyEffect(sNewName);
			m_kEffectList.erase(m_kEffectList.begin()+i);
			break;
		}
	}
}

KVOID Kylin::Node::ActivateEffect( KSTR sName, KBOOL bFlag )
{
	KSTR sNewName = sName + Ogre::StringConverter::toString(GetWorldID());
	EffectManager::GetSingletonPtr()->Activate(sNewName, bFlag);
}

KVOID Kylin::Node::SetRotation( KQuaternion kQua )
{
	m_pOgreNode->setOrientation(kQua);
}

KQuaternion Kylin::Node::GetRotation()
{
	return m_pOgreNode->getOrientation();
}

KBOOL Kylin::Node::AttachMesh( Ogre::Entity* pEnt, KSTR sBone )
{

	if (m_pOgreEntity->getSkeleton()->hasBone(sBone))
	{
		pEnt->detachFromParent();

		if (m_pOgreEntity->attachObjectToBone(sBone,pEnt))
			return true;
	}

	AssertEx(NULL,"骨骼顶点不存在！");
	return false;
}

KVOID Kylin::Node::DetachMesh( Ogre::Entity* pEnt )
{
	m_pOgreEntity->detachObjectFromBone(pEnt);
}

KVOID Kylin::Node::SetTransparency( KFLOAT fAlph )
{
	SAFE_CALL(m_pTransparency,setTransparency(fAlph));
}

KFLOAT Kylin::Node::GetTransparency()
{
	return m_pTransparency->getTransparency();
}