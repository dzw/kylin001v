#include "engpch.h"
#include "CollisionWrapper.h"


Kylin::CollisionWrapper::CollisionWrapper()
: m_bDispCllsnFrame(false)
{

}

Kylin::CollisionWrapper::~CollisionWrapper()
{
	Destroy();
}

KBOOL Kylin::CollisionWrapper::Initialize(SceneManager* pSceneMgr)
{
// 	if (!CollisionManager::getSingletonPtr())
// 		KNEW CollisionManager(pSceneMgr);
// 
// 	CollisionManager::getSingletonPtr()->addCollClass("entity");
// 	CollisionManager::getSingletonPtr()->addCollClass("camera");
// 	CollisionManager::getSingletonPtr()->addCollClass("terrain");
// 
// 	CollisionManager::getSingletonPtr()->addCollType("entity", "entity", COLLTYPE_EXACT);
// 	CollisionManager::getSingletonPtr()->addCollType("entity", "terrain", COLLTYPE_EXACT);
// 	CollisionManager::getSingletonPtr()->addCollType("camera", "camera", COLLTYPE_IGNORE);
// 	CollisionManager::getSingletonPtr()->addCollType("entity", "camera", COLLTYPE_IGNORE);
// 	CollisionManager::getSingletonPtr()->addCollType("camera", "terrain", COLLTYPE_IGNORE);
// 	CollisionManager::getSingletonPtr()->addCollType("terrain", "terrain", COLLTYPE_IGNORE);

	return true;
}

KVOID Kylin::CollisionWrapper::Update( KFLOAT fElapsed )
{
// 	OgreOpcode::CollisionContext* pCollideContext = CollisionManager::getSingletonPtr()->getDefaultContext();
// 
// 	if (m_bDispCllsnFrame)
// 		pCollideContext->visualize(true, true, true, true, false, true);
// 
// 	pCollideContext->collide(fElapsed);
}

KVOID Kylin::CollisionWrapper::Destroy()
{
//	KDEL CollisionManager::getSingletonPtr();
}

KINT Kylin::CollisionWrapper::Query( const Ogre::Ray kCollideRay ,KFLOAT fCllsnDist)
{
// 	OgreOpcode::CollisionContext* pCollideContext = CollisionManager::getSingletonPtr()->getDefaultContext();
// 	OgreOpcode::CollisionPair **ppPickReport;
// 	int nPicks = pCollideContext->rayCheck(kCollideRay, fCllsnDist, COLLTYPE_QUICK, COLLTYPE_ALWAYS_QUICK, ppPickReport);
// 	if (nPicks > 0)
// 	{
// 		for(int i = 0; i < nPicks; i++)
// 		{
// 			//ppPickReport[0]->this_object->getName();
// 			Kylin::CollisioListener* pCollider = FindCollider(ppPickReport[i]->this_object->getName());
// 			Kylin::CollisioListener* pCollidee = FindCollider(ppPickReport[i]->other_object->getName());
// 			if (pCollider)
// 			{
// 				Kylin::Node* pOther = (Kylin::Node*)pCollidee;
// 				if (pCollider->OnShouldCllsn(pOther))
// 					pCollider->OnEntityCllsn(pOther,ppPickReport[i]->this_normal);
// 			}
// 		}
// 	}
// 
// 	return nPicks;
	return 0;
}

KVOID Kylin::CollisionWrapper::ShowCllsnFrame( KBOOL bFlag )
{
	m_bDispCllsnFrame = bFlag;
}

KVOID Kylin::CollisionWrapper::RegisterCollider( CollisioListener* pCollider )
{
	ColliderList::iterator beg = m_kColliders.begin();
	ColliderList::iterator end = m_kColliders.end();
	
	for (ColliderList::iterator it = beg; it != end; it++)
	{
		if (pCollider == *it)
			return;
	}

	m_kColliders.push_back(pCollider);
}

KVOID Kylin::CollisionWrapper::UnregisterCollider( CollisioListener* pCollider )
{
	ColliderList::iterator beg = m_kColliders.begin();
	ColliderList::iterator end = m_kColliders.end();

	for (ColliderList::iterator it = beg; it != end; it++)
	{
		if (pCollider == *it)
		{
			m_kColliders.erase(it);
			break;
		}
	}
}

Kylin::CollisioListener* Kylin::CollisionWrapper::FindCollider( KSTR sName )
{
	ColliderList::iterator beg = m_kColliders.begin();
	ColliderList::iterator end = m_kColliders.end();

	for (ColliderList::iterator it = beg; it != end; it++)
	{
		if (sName == (*it)->GetName())
		{
			return *it;
		}
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////
//
KBOOL Kylin::CollisioListener::SetupCllsn( Ogre::Entity* pEnt,Kylin::PropertySet kProp )
{
	if (!pEnt) return false;

	m_sName = pEnt->getName();

	KUINT uType;
	KUINT uShape;
	
// 	if (!kProp.GetUIntValue("$CLLSN_SHAPE",uShape)) return false;
// 	if (!kProp.GetUIntValue("$CLLSN_TYPE",uType)) return false;
// 
// 	switch (uShape)
// 	{
// 	case CLS_ENTITY:
// 		{
// 			m_pCllsnShape = CollisionManager::getSingletonPtr()->createEntityCollisionShape(m_sName);
// 			EntityCollisionShape* pShape = static_cast<EntityCollisionShape*>(m_pCllsnShape);
// 			if (!pShape->load(pEnt)) return false;
// 		}
// 
// 		break;
// 	case CLS_BOX:
// 		m_pCllsnShape = CollisionManager::getSingletonPtr()->createBoxCollisionShape(m_sName);
// 		break;
// 	case CLS_TERRAIN:
// 		m_pCllsnShape = CollisionManager::getSingletonPtr()->createTerrainCollisionShape(m_sName);
// 		break;
// 	case CLS_MESH:
// 		m_pCllsnShape = CollisionManager::getSingletonPtr()->createMeshCollisionShape(m_sName);
// 		break;
// 	case CLS_CAPSULE_MESH:
// 		m_pCllsnShape = CollisionManager::getSingletonPtr()->createCapsuleMeshCollisionShape(m_sName);
// 		break;
// 	case CLS_SPHERE_MESH:
// 		m_pCllsnShape = CollisionManager::getSingletonPtr()->createSphereMeshCollisionShape(m_sName);
// 		break;
// 	}
// 	
// 	//////////////////////////////////////////////////////////////////////////
// 	CollisionContext* pCollideContext = CollisionManager::getSingletonPtr()->getDefaultContext();
// 	assert(pCollideContext);
// 	
// 	m_pCllsnObject = pCollideContext->createObject(m_sName);
// 
// 	if (uType == CLT_ENTITY)
// 		m_pCllsnObject->setCollClass("entity");
// 	else if (uType == CLT_CAMERA)
// 		m_pCllsnObject->setCollClass("camera");
// 	else
// 		m_pCllsnObject->setCollClass("terrain");
// 
// 	m_pCllsnObject->setShape(m_pCllsnShape);
// 	pCollideContext->addObject(m_pCllsnObject);
// 
// 	pCollideContext->reset();
	//////////////////////////////////////////////////////////////////////////
	CollisionWrapper::GetSingletonPtr()->RegisterCollider(this);

	return true;
}


Kylin::CollisioListener::~CollisioListener()
{
	if (CollisionWrapper::Initialized())
		CollisionWrapper::GetSingletonPtr()->UnregisterCollider(this);
}