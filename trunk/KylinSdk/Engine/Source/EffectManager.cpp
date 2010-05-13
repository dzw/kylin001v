#include "engpch.h"
#include "EffectManager.h"
#include "rOgreRoot.h"
#include "ParticleUniverseSystemManager.h"


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
	PropertySet kInfo;
	kInfo.SetValue("$Name",KSTR("Bloom"));
	Generate(kInfo,ET_COMPOSITOR);
	Generate(KNEW EffectMotionBlur());

	return true;
}

Kylin::EffectObject* Kylin::EffectManager::Generate( PropertySet& kInfo, KUINT uType)
{
	KSTR sName = "";
	if (!kInfo.GetStrValue("$Name",sName))
		return NULL;
	
	EffectMap::iterator it = m_kEffectMap.find(sName);
	if (it != m_kEffectMap.end())
		return it->second;

	EffectObject* pEffect = NULL;
	if (uType == ET_PARTICLE)
	{
		KSTR sTemplate = "";
		if (!kInfo.GetStrValue("$Template",sTemplate))
			return NULL;

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

KVOID Kylin::EffectParticle::Attach( Ogre::SceneNode* pNode )
{
	assert(pNode);
	if (m_pSystem->getParentSceneNode())
		m_pSystem->getParentSceneNode()->detachObject(m_pSystem);
	pNode->attachObject(m_pSystem);
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


//////////////////////////////////////////////////////////////////////////

KBOOL Kylin::EffectMotionBlur::Initialize()
{
	m_sName = "MotionBlur";

	/// Motion blur effect
	Ogre::CompositorPtr comp3 = Ogre::CompositorManager::getSingleton().create(
		m_sName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);
	{
		Ogre::CompositionTechnique *t = comp3->createTechnique();
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("scene");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("sum");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("temp");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		/// Render scene
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("scene");
		}
		/// Initialisation pass for sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("sum");
			tp->setOnlyInitial(true);
		}
		/// Do the motion blur
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("temp");
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/Combine");
			pass->setInput(0, "scene");
			pass->setInput(1, "sum");
			}
		}
		/// Copy back sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("sum");
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/Copyback");
			pass->setInput(0, "temp");
			}
		}
		/// Display result
		{
			Ogre::CompositionTargetPass *tp = t->getOutputTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/MotionBlur");
			pass->setInput(0, "sum");
			}
		}
	}
	
	return EffectCompositor::Initialize();
}
