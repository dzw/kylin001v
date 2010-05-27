#include "engpch.h"
#include "EffectManager.h"
#include "rOgreRoot.h"
#include "ParticleUniverseSystemManager.h"
#include "EffectMotionBlur.h"
#include "EffectFade.h"


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

	Generate("Bloom","",ET_COMPOSITOR);
	Generate(KNEW EffectFade());
	Generate(KNEW EffectMotionBlur());

	return true;
}

Kylin::EffectObject* Kylin::EffectManager::Generate( const KSTR& sName,const KSTR& sTemplate, KUINT uType)
{
	EffectMap::iterator it = m_kEffectMap.find(sName);
	if (it != m_kEffectMap.end())
		return it->second;

	EffectObject* pEffect = NULL;
	if (uType == ET_PARTICLE)
	{
		pEffect = KNEW EffectParticle(sName,sTemplate);		
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

KVOID Kylin::EffectManager::Generate( EffectObject* pEffect )
{
	if (pEffect)
	{
		pEffect->Initialize();
		m_kEffectMap.insert(std::pair<KSTR,EffectObject*>(pEffect->GetName(),pEffect));
	}
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
		SAFE_DEL(it->second);
	}

	m_kEffectMap.clear();
	//////////////////////////////////////////////////////////////////////////
	KDEL ParticleUniverse::ParticleSystemManager::getSingletonPtr();
}

KVOID Kylin::EffectManager::DestroyEffect( KSTR sName )
{
	EffectMap::iterator it = m_kEffectMap.find(sName);
	if (it != m_kEffectMap.end())
	{
		SAFE_DEL(it->second);
		m_kEffectMap.erase(it);
	}
}

KVOID Kylin::EffectManager::Render( KFLOAT fElapsed )
{
	EffectMap::iterator beg = m_kEffectMap.begin();
	EffectMap::iterator end = m_kEffectMap.end();
	for (EffectMap::iterator it = beg; it != end; it++)
	{
		if (it->second->IsEnabled())
			it->second->Render(fElapsed);
	}
}

//////////////////////////////////////////////////////////////////////////
Kylin::EffectParticle::EffectParticle( KSTR sName, KSTR sTemplate)
: EffectObject(sName)
, m_sTemplate(sTemplate)
, m_pSystem(NULL)
{
	m_uType = EffectManager::ET_PARTICLE;
}

Kylin::EffectParticle::~EffectParticle()
{
	Destroy();
}

KBOOL Kylin::EffectParticle::Initialize()
{
	ParticleUniverse::ParticleSystemManager* pPSManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr(); 
	Ogre::SceneManager* pSceneMngr = OgreRoot::GetSingletonPtr()->GetSceneManager();

	m_pSystem = pPSManager->getParticleSystem(m_sName);
	if (!m_pSystem)
		m_pSystem = pPSManager->createParticleSystem(m_sName, m_sTemplate, pSceneMngr);
	
	return true;
}

KVOID Kylin::EffectParticle::Destroy()
{
	ParticleUniverse::ParticleSystemManager* pPSManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr(); 
	Ogre::SceneManager* pSceneMngr = OgreRoot::GetSingletonPtr()->GetSceneManager();
	
	if (m_pSystem->getParentSceneNode())
		m_pSystem->getParentSceneNode()->detachObject(m_pSystem);

	pPSManager->destroyParticleSystem(m_pSystem,pSceneMngr);
	m_pSystem = NULL;
}

KVOID Kylin::EffectParticle::Activate( KBOOL bFlag )
{
	if (m_pSystem)
	{
		if (bFlag)
		{
			if (m_pSystem->getState() != ParticleUniverse::ParticleSystem::PSS_STARTED)
				m_pSystem->start();
		}
		else if (m_pSystem->getState() != ParticleUniverse::ParticleSystem::PSS_STOPPED)
			m_pSystem->stop();
	}
}

KVOID Kylin::EffectParticle::Attach( Ogre::SceneNode* pNode, KPoint3 kOffset )
{
	assert(pNode);
	if (m_pSystem->getParentSceneNode())
		m_pSystem->getParentSceneNode()->detachObject(m_pSystem);
	pNode->attachObject(m_pSystem);

	pNode->setPosition(kOffset);
}

KVOID Kylin::EffectParticle::SetScale( KFLOAT fScale )
{
	SAFE_CALL(m_pSystem,setScale(KPoint3(fScale,fScale,fScale)));
}

//////////////////////////////////////////////////////////////////////////

Kylin::EffectCompositor::EffectCompositor( KSTR sName )
: EffectObject(sName)
{
	m_uType = EffectManager::ET_COMPOSITOR;
}

Kylin::EffectCompositor::~EffectCompositor()
{
	Destroy();
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

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//KBOOL Kylin::EffectBloom::Initialize()
//{
	/// Bloom effect
//	Ogre::CompositorPtr spCompositor = Ogre::CompositorManager::getSingleton().create(
//		m_sName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
//		);

// 	CompositionTechnique *t;
// 	CompositionTechnique::TextureDefinition *td;
// 	CompositionTargetPass *tp;
// 	CompositionPass *pass;
// 
// 	t = spCompositor->createTechnique();
// 	td = t->createTextureDefinition("scene");
// 	td->width = 1;
// 	td->height = 1;
// 	td->formatList.push_back(PF_X8R8G8B8);
// 	td = t->createTextureDefinition("blur0");
// 	td->width = 0.25;
// 	td->height = 0.25;
// 	td->formatList.push_back(PF_X8R8G8B8);
// 	td = t->createTextureDefinition("blur1");
// 	td->width = 0.25;
// 	td->height = 0.25;
// 	td->formatList.push_back(PF_X8R8G8B8);
// 
// 	tp = t->createTargetPass();
// 	tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
// 	tp->setOutputName("scene");
// 
// 	tp = t->createTargetPass();
// 	tp->setInputMode(CompositionTargetPass::IM_NONE);
// 	tp->setOutputName("blur0");
// 	pass = tp->createPass();
// 	pass->setType(CompositionPass::PT_RENDERQUAD);
// 	pass->setMaterialName("PostFilters/Bloom/Blur");
// 	pass->setInput(0, "scene");
// 
// 	tp = t->createTargetPass();
// 	tp->setInputMode(CompositionTargetPass::IM_NONE);
// 	tp->setOutputName("blur1");
// 	pass = tp->createPass();
// 	pass->setType(CompositionPass::PT_RENDERQUAD);
// 	pass->setMaterialName("PostFilters/Bloom/Blur");
// 	pass->setInput(0, "blur0");
// 
// 	tp = t->getOutputTargetPass();
// 	tp->setInputMode(CompositionTargetPass::IM_NONE);
// 	pass = tp->createPass();
// 	pass->setType(CompositionPass::PT_RENDERQUAD);
// 	pass->setMaterialName("PostFilters/Bloom/Final");
// 	pass->setInput(0, "scene");
// 	pass->setInput(1, "blur1");
// 
// 	return EffectCompositor::Initialize();
//}