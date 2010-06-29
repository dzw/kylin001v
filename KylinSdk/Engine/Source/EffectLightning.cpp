#include "engpch.h"
#include "EffectLightning.h"

#include <cstdlib>
#include <boost/random.hpp>
#include "rOgreRoot.h"


Kylin::EffectLightning::EffectLightning(KSTR sName)
: EffectObject(sName)
, m_pBillboardChain(NULL)
, m_pSceneNode(NULL)
{
	
}

KBOOL Kylin::EffectLightning::Initialize()
{
	m_pBillboardChain = OgreRoot::GetSingletonPtr()->GetSceneManager()->createBillboardChain(m_sName);
	m_pBillboardChain->setMaterialName("lightning");

	m_pSceneNode = OgreRoot::GetSingletonPtr()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
	m_pSceneNode->attachObject(m_pBillboardChain);

	//mBillboardChain->setUseTextureCoords(true);
	m_pBillboardChain->setUseVertexColours(true);

	return true;
}

KVOID Kylin::EffectLightning::Attach( const KPoint3& kStart,const KPoint3& kEnd )
{
	if (!m_pBillboardChain)
		return;

	mStartPos	= kStart;
	mEndPos		= kEnd;

	const KPoint3 distance = mEndPos - mStartPos;
	
	KPoint3 element_position	= mStartPos;
	Ogre::Real element_width		= 1;
	Ogre::Real texcoord				= 0;
	KPoint3 step				= distance/19;
	for (int i = 0; i<20; ++i)
	{
		//texcoord = i;
		m_pBillboardChain->addChainElement(0, Ogre::BillboardChain::Element(element_position, 
			element_width,texcoord, Ogre::ColourValue::White));
		element_position += step;
		element_width += 0.158;
	}
}

KVOID Kylin::EffectLightning::Render(KFLOAT fElapsed)
{
	const KPoint3 distance = mEndPos - mStartPos;

	// update position and jitter
	KPoint3 element_position = mStartPos;
	KPoint3 step = distance/19;
	for (int i = 0; i<20; ++i)
	{
		Ogre::BillboardChain::Element element = m_pBillboardChain->getChainElement(0,i);

		KPoint3 random_vector(0,0,0);
		if ((i!=0)&&(i!=19))
		{
			const Ogre::Real ceil = 5;
			random_vector = KPoint3((static_cast<float>(rand()) / RAND_MAX) * ceil,
				(static_cast<float>(rand()) / RAND_MAX) * ceil,
				(static_cast<float>(rand()) / RAND_MAX) * ceil);
		}
		element.position = step.normalisedCopy().crossProduct(random_vector) + element_position;
		m_pBillboardChain->updateChainElement(0,i,element);
		element_position += step;
	}

}

KVOID Kylin::EffectLightning::Destroy()
{
	m_pSceneNode->removeAndDestroyAllChildren();
	OgreRoot::GetSingletonPtr()->GetSceneManager()->destroySceneNode(m_pSceneNode);

	m_pBillboardChain	= NULL;
	m_pSceneNode		= NULL;
}

KVOID Kylin::EffectLightning::Activate( KBOOL bFlag )
{
	SAFE_CALL(m_pBillboardChain,setVisible(bFlag));

}

KBOOL Kylin::EffectLightning::IsVisible()
{
	return m_pBillboardChain->isVisible();
}