#include "engpch.h"
#include "SkyXWapper.h"

// ---------------------------------------------------------------------------
// Include SkyX header files
// ---------------------------------------------------------------------------
#include "SkyX.h"


Kylin::SkyXWapper::SkyXWapper(Ogre::SceneManager* pSceneMnger, Ogre::Camera* pCamera)
: m_pSkyX(NULL)
{
	// Create SkyX
	m_pSkyX = KNEW SkyX::SkyX(pSceneMnger, pCamera);
	
	m_pSkyX->create();

	// Volumetric clouds
	//m_pSkyX->getVCloudsManager()->create();

	// No smooth fading
	m_pSkyX->getMeshManager()->setSkydomeFadingParameters(true);

	// A little change to default atmosphere settings :)
	SkyX::AtmosphereManager::Options atOpt = m_pSkyX->getAtmosphereManager()->getOptions();
	atOpt.RayleighMultiplier = 0.0045f;
	atOpt.MieMultiplier = 0.00125f;
	atOpt.InnerRadius = 9.92f;
	atOpt.OuterRadius = 10.3311f;
	m_pSkyX->getAtmosphereManager()->setOptions(atOpt);

	// Add our ground atmospheric scattering pass to terrain material
	//Ogre::Terrain* pTer = terGroup->getTerrain(0, 0);
	//Ogre::MaterialPtr spMat = pTer->getMaterial();

	// Add a basic cloud layer
	m_pSkyX->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
}


Kylin::SkyXWapper::~SkyXWapper()
{
	SAFE_DEL(m_pSkyX);
}

KVOID Kylin::SkyXWapper::Tick( KFLOAT fElapsed )
{
	
	if (m_pSkyX)
	{
		m_pSkyX->setTimeMultiplier(0.1f);
		
		if (m_pSkyX->getVCloudsManager()->getVClouds())
		{
			if (m_pSkyX->getVCloudsManager()->getVClouds()->getWindDirection().valueDegrees() < 0)
			{
				m_pSkyX->getVCloudsManager()->getVClouds()->setWindDirection(Ogre::Degree(0));
			}
			if (m_pSkyX->getVCloudsManager()->getVClouds()->getWindDirection().valueDegrees() > 360)
			{
				m_pSkyX->getVCloudsManager()->getVClouds()->setWindDirection(Ogre::Degree(360));
			}
		}
		
		m_pSkyX->update(fElapsed);
	}
}

KVOID Kylin::SkyXWapper::AddGroundPass( Ogre::MaterialPtr spMat )
{

	m_pSkyX->getGPUManager()->addGroundPass(spMat->getTechnique(0)->createPass(), 
		5000, 
		Ogre::SBT_TRANSPARENT_COLOUR
		);
}
