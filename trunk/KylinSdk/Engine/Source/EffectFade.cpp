#include "engpch.h"
#include "EffectFade.h"



KBOOL Kylin::EffectFade::Initialize()
{

	Ogre::CompositorPtr spCompositor = Ogre::CompositorManager::getSingleton().create(
		m_sName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);

	Ogre::CompositionTechnique *t;
	Ogre::CompositionTechnique::TextureDefinition *td;
	Ogre::CompositionTargetPass *tp;
	Ogre::CompositionPass *pass;

	t = spCompositor->createTechnique();
	td = t->createTextureDefinition("scene");
	td->width = 1;
	td->height = 1;
	td->formatList.push_back(Ogre::PF_X8R8G8B8);

	tp = t->createTargetPass();
	tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
	tp->setOutputName("scene");

	tp = t->getOutputTargetPass();
	tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
	pass = tp->createPass();
	pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
	pass->setMaterialName("PostFilters/Fade");
	pass->setInput(0, "scene");
	pass->setIdentifier(0xDEADBADE);

	// receive default parameters from material script
	m_fColourAmount = 0;
	m_fGrayAmount	= 1;
	if (!pass->getMaterial().isNull())
	{
		Ogre::GpuProgramParametersSharedPtr parameters = pass->getMaterial()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
/*#if OGRE_VERSION >= 0x010300*/
		const Ogre::GpuConstantDefinition* def;

		def = parameters->_findNamedConstantDefinition("colour_amount");
		if (def)
			parameters->_readRawConstants(def->physicalIndex, 1, &m_fColourAmount);
		def = parameters->_findNamedConstantDefinition("gray_amount");
		if (def)
			parameters->_readRawConstants(def->physicalIndex, 1, &m_fGrayAmount);
// #else
// 		GpuProgramParameters::RealConstantEntry* entry;
// 
// 		entry = parameters->getNamedRealConstantEntry("colour_amount");
// 		if (entry && entry->isSet)
// 			m_fColourAmount = entry->val[0];
// 
// 		entry = parameters->getNamedRealConstantEntry("gray_amount");
// 		if (entry && entry->isSet)
// 			m_fGrayAmount = entry->val[0];
// #endif
	}

	KBOOL bRet = EffectCompositor::Initialize();
	
	if (bRet)
		m_pCompositor->addListener(this);

	return bRet;
}

KVOID Kylin::EffectFade::Render( KFLOAT fElapsed )
{
	EffectCompositor::Render(fElapsed);

	if (m_pCompositor->getEnabled() )
	{
		if(m_bActived && m_nDelay < 100)
		{
			m_nDelay += 1;

			m_fColourAmount = (100.0f-m_nDelay) * 0.01f;
			m_fGrayAmount	= (KFLOAT)(m_nDelay * 0.01f);
		}
		else if(!m_bActived && m_nDelay > 0)
		{
			m_nDelay -= 1;

			m_fColourAmount = (100.0f-m_nDelay) * 0.01f;
			m_fGrayAmount	= (KFLOAT)(m_nDelay * 0.01f);

			if(m_nDelay <= 0)
			{
				Activate(false);
			}
		}
	}
}

KVOID Kylin::EffectFade::Activate( KBOOL bFlag )
{
	EffectCompositor::Activate(bFlag);

	m_bActived		= bFlag;
	m_nDelay		= 0;

	m_fColourAmount = 0;
	m_fGrayAmount	= 1;
}

KVOID Kylin::EffectFade::Destroy()
{
	m_pCompositor->removeListener(this);

	EffectCompositor::Destroy();
}