#include "engpch.h"
#include "EffectManager.h"
#include "rOgreRoot.h"
//#include "ParticleUniverseSystemManager.h"
#include "EffectMotionBlur.h"
#include "EffectFade.h"
#include "ClockingCallback.h"


KBOOL Kylin::EffectManager::Initialize()
{
	// do not implement if not supported
// 	const Ogre::RenderSystemCapabilities* caps = pRoot->getRenderSystem()->getCapabilities();
// 	if ( caps->hasCapability(Ogre::RSC_VERTEX_PROGRAM) 
// 		&& (caps->hasCapability(Ogre::RSC_FRAGMENT_PROGRAM))
// 		)
// 	{
// 		// just experimenting with compositors here
// 		CreateCompositors();
// 	}
	//KNEW ParticleUniverse::ParticleSystemManager();
	Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);

	Generate("Bloom","",-1.0f,ET_COMPOSITOR);
	Generate(KNEW EffectFade());
	Generate(KNEW EffectMotionBlur());
	
	//
	Activate("Bloom",true);

	return true;
}

Kylin::EffectObject* Kylin::EffectManager::Generate( const KSTR& sName,const KSTR& sTemplate, KFLOAT fLifeTime, KUINT uType)
{
	EffectMap::iterator it = m_kEffectMap.find(sName);
	if (it != m_kEffectMap.end())
		return it->second;

	EffectObject* pEffect = NULL;
	if (uType == ET_PARTICLE)
	{
		pEffect = KNEW EffectParticle(sName,sTemplate,fLifeTime);		
		pEffect->Initialize();
	}
	else if (uType == ET_COMPOSITOR)
	{
		pEffect = KNEW EffectCompositor(sName);
		pEffect->Initialize();
	}

	if (pEffect)
		m_kEffectMap.insert(std::pair<KSTR,EffectObject*>(sName,pEffect));

	return pEffect;
}

Kylin::EffectObject* Kylin::EffectManager::Generate( EffectObject* pEffect )
{
	if (pEffect)
	{
		pEffect->Initialize();
		m_kEffectMap.insert(std::pair<KSTR,EffectObject*>(pEffect->GetName(),pEffect));
	}

	return pEffect;
}

KVOID Kylin::EffectManager::Activate( KSTR sName, KBOOL bFlag)
{
	EffectMap::iterator it = m_kEffectMap.find(sName);
	if (it != m_kEffectMap.end())
	{
		it->second->Activate(bFlag);
	}
}

KVOID Kylin::EffectManager::Destroy()
{
	EffectMap::iterator beg = m_kEffectMap.begin();
	EffectMap::iterator end = m_kEffectMap.end();
	for (EffectMap::iterator it = beg; it != end; it++)
	{
		SAFE_CALL(it->second,Destroy());
		SAFE_DEL(it->second);
	}

	m_kEffectMap.clear();
	//////////////////////////////////////////////////////////////////////////
	//KDEL ParticleUniverse::ParticleSystemManager::getSingletonPtr();
}

KVOID Kylin::EffectManager::DestroyEffect( KSTR sName )
{
	EffectMap::iterator it = m_kEffectMap.find(sName);
	if (it != m_kEffectMap.end())
	{
		m_kWaitList.push_back(sName);
	}
}

KVOID Kylin::EffectManager::Render( KFLOAT fElapsed )
{
	if (m_kWaitList.size() > 0)
	{
		for (KUINT i = 0; i < m_kWaitList.size(); i++)
		{
			EffectMap::iterator it = m_kEffectMap.find(m_kWaitList[i]);
			if (it != m_kEffectMap.end())
			{
				SAFE_CALL(it->second,Destroy());
				SAFE_DEL(it->second);
				m_kEffectMap.erase(it);
			}
		}

		m_kWaitList.clear();
	}


	EffectMap::iterator beg = m_kEffectMap.begin();
	EffectMap::iterator end = m_kEffectMap.end();
	for (EffectMap::iterator it = beg; it != end; it++)
	{
		if (it->second->IsVisible())
			it->second->Render(fElapsed);
	}
}

//////////////////////////////////////////////////////////////////////////
KVOID Kylin::EffectObject::SetCallbackObj( ClockingCallback* pObj )
{
	m_pClocking = pObj;
}

//////////////////////////////////////////////////////////////////////////
Kylin::EffectParticle::EffectParticle( KSTR sName, KSTR sTemplate, KFLOAT fLifeTime)
: EffectObject(sName)
, m_pRoot(NULL)
, m_sTemplate(sTemplate)
, m_fLifeTime(fLifeTime)
, m_pParticleHandle(NULL)
//, m_pParticleSystemEx(NULL)
{
	m_uType = ET_PARTICLE;
}

Kylin::EffectParticle::~EffectParticle()
{
	//Destroy();
}

KBOOL Kylin::EffectParticle::Initialize()
{
	Ogre::SceneManager* pSceneMngr = OgreRoot::GetSingletonPtr()->GetSceneManager();
	
	if ( pSceneMngr->hasParticleSystem(m_sName) )
		m_pParticleHandle = pSceneMngr->getParticleSystem(m_sName);
	else
		m_pParticleHandle = pSceneMngr->createParticleSystem(m_sName, m_sTemplate);
	
	m_pParticleHandle->setRenderingDistance(VISIBLE_DISTANCE);
	m_pParticleHandle->setCastShadows(false);
	// 若要使粒子特效缩放有效请打开此项
	m_pParticleHandle->setKeepParticlesInLocalSpace(true);
	
	m_pRoot = pSceneMngr->getRootSceneNode()->createChildSceneNode();
	m_pRoot->attachObject(m_pParticleHandle);

	Activate(false);
// 	ParticleUniverse::ParticleSystemManager* pPSManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr(); 
// 	
// 	m_pParticleSystemEx = pPSManager->getParticleSystem(m_sName);
// 	if (!m_pParticleSystemEx)
// 		m_pParticleSystemEx = pPSManager->createParticleSystem(m_sName, m_sTemplate, pSceneMngr);
	
	return true;
}

KVOID Kylin::EffectParticle::Destroy()
{
// 	ParticleUniverse::ParticleSystemManager* pPSManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr(); 
// 	Ogre::SceneManager* pSceneMngr = OgreRoot::GetSingletonPtr()->GetSceneManager();
// 	
// 	if (m_pParticleSystemEx->getParentSceneNode())
// 		m_pParticleSystemEx->getParentSceneNode()->detachObject(m_pParticleSystemEx);
// 
// 	pPSManager->destroyParticleSystem(m_pParticleSystemEx,pSceneMngr);
// 	m_pParticleSystemEx = NULL;

	Ogre::SceneManager* pSceneMngr = OgreRoot::GetSingletonPtr()->GetSceneManager();

	m_pParticleHandle->detachFromParent();
	pSceneMngr->destroyParticleSystem(m_pParticleHandle);
	m_pParticleHandle = NULL;
	
	if (pSceneMngr->hasSceneNode(m_pRoot->getName()))
		pSceneMngr->destroySceneNode(m_pRoot);

	m_pRoot = NULL;
}

KVOID Kylin::EffectParticle::Activate( KBOOL bFlag )
{
// 	if (m_pParticleSystemEx)
// 	{
// 		if (bFlag)
// 		{
// 			if (m_pParticleSystemEx->getState() != ParticleUniverse::ParticleSystem::PSS_STARTED)
// 				m_pParticleSystemEx->start();
// 		}
// 		else if (m_pParticleSystemEx->getState() != ParticleUniverse::ParticleSystem::PSS_STOPPED)
// 			m_pParticleSystemEx->stop();
// 	}

	SAFE_CALL(m_pParticleHandle,setVisible(bFlag));

}

KVOID Kylin::EffectParticle::Attach( Ogre::SceneNode* pNode, KFLOAT fScale)
{
	Assert(pNode);
// 	if (m_pParticleSystemEx->getParentSceneNode())
// 		m_pParticleSystemEx->getParentSceneNode()->detachObject(m_pParticleSystemEx);
// 	pNode->attachObject(m_pParticleSystemEx);

	if (m_pRoot->getParent())
		m_pRoot->getParent()->removeChild(m_pRoot);

	pNode->addChild(m_pRoot);
	m_pRoot->setScale(KPoint3(fScale,fScale,fScale));
}

KVOID Kylin::EffectParticle::SetScale( KFLOAT fScale )
{
	//SAFE_CALL(m_pParticleSystemEx,setScale(KPoint3(fScale,fScale,fScale)));
	SAFE_CALL(m_pRoot,setScale(KPoint3(fScale,fScale,fScale)));
}

KVOID Kylin::EffectParticle::Render( KFLOAT fElapsed )
{
	if (m_fLifeTime > .0f)
	{
		m_fLifeTime -= fElapsed;
		if (m_fLifeTime <= .0f)
		{
			SAFE_CALL(m_pClocking,EndTime(CLASS_TO(EffectParticle),m_sTemplate,m_kUserData));

			if (m_bAutoRemove && EffectManager::Initialized())
				EffectManager::GetSingletonPtr()->DestroyEffect(m_sName);
		}
	}
}

KBOOL Kylin::EffectParticle::IsVisible()
{
	return m_pParticleHandle->getVisible();
}

KVOID Kylin::EffectParticle::SetPosition( const KPoint3& pt )
{
	SAFE_CALL(m_pRoot,setPosition(pt));
}
//////////////////////////////////////////////////////////////////////////

Kylin::EffectCompositor::EffectCompositor( KSTR sName )
: EffectObject(sName)
{
	m_uType = ET_COMPOSITOR;
}

Kylin::EffectCompositor::~EffectCompositor()
{
	//Destroy();
}


KBOOL Kylin::EffectCompositor::Initialize()
{
	Ogre::Viewport* pVP = OgreRoot::GetSingletonPtr()->GetMainWindow()->getViewport(0);

	m_pCompositor = Ogre::CompositorManager::getSingletonPtr()->addCompositor(pVP, m_sName);
	m_pCompositor->setEnabled(false);

	return true;
}

KVOID Kylin::EffectCompositor::Destroy()
{
	Ogre::Viewport* pVP = OgreRoot::GetSingletonPtr()->GetMainWindow()->getViewport(0);
	Ogre::CompositorManager::getSingletonPtr()->removeCompositor(pVP,m_sName);
}

KVOID Kylin::EffectCompositor::Activate( KBOOL bFlag )
{
	if (m_pCompositor)
			m_pCompositor->setEnabled(bFlag);
}

KBOOL Kylin::EffectCompositor::IsEnabled()
{
	return m_pCompositor->getEnabled();
}

