#include "engpch.h"
#include "efLightning.h"

#include <cstdlib>
#include <boost/random.hpp>

Kylin::Lightning::Lightning(Ogre::SceneManager* scene_manager) : GameObject("Lightning", scene_manager)
{
	mSceneManager = scene_manager;
}

Kylin::Lightning::~Lightning()
{

}

void Kylin::Lightning::AddToScene(const Ogre::Vector3 &position /* = Ogre::Vector3::ZERO  */,
						   const Ogre::Quaternion &orientation /* = Ogre::Quaternion::IDENTITY */)
{
	AddToScene(mSceneNode, mSceneNode);
}

void Kylin::Lightning::AddToScene(Ogre::SceneNode *start_node, Ogre::SceneNode *end_node)
{
	mStartNode = start_node;
	mEndNode = end_node;
	mStartPosition = mStartNode->getPosition();
	mEndPosition = mEndNode->getPosition();

	const Ogre::Vector3 distance = mEndPosition - mStartPosition;

	mBillboardChain = new Ogre::BillboardChain(mName);
	mBillboardChain->setMaterialName("lightning");

	mSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode(mName);
	mSceneNode->attachObject(mBillboardChain);

	//mBillboardChain->setUseTextureCoords(true);
	mBillboardChain->setUseVertexColours(true);

	Ogre::Vector3 element_position = mStartPosition;
	Ogre::Real element_width = 1;
	Ogre::Real texcoord = 0;
	Ogre::Vector3 step = distance/19;
	for (int i = 0; i<20; ++i)
	{
		//texcoord = i;
		mBillboardChain->addChainElement(0, Ogre::BillboardChain::Element(element_position, 
			element_width,texcoord, Ogre::ColourValue::White));
		element_position += step;
		element_width += 0.158;
	}
}

void Kylin::Lightning::RemoveFromScene()
{

}

bool Kylin::Lightning::UpdateLogic(Ogre::Real time_step)
{
	mStartPosition = mStartNode->getPosition();
	mEndPosition = mEndNode->getPosition();

	const Ogre::Vector3 distance = mEndPosition - mStartPosition;

	// update position and jitter
	Ogre::Vector3 element_position = mStartPosition;
	Ogre::Vector3 step = distance/19;
	for (int i = 0; i<20; ++i)
	{
		Ogre::BillboardChain::Element element = mBillboardChain->getChainElement(0,i);

		Ogre::Vector3 random_vector(0,0,0);
		if ((i!=0)&&(i!=19))
		{
			const Ogre::Real ceil = 5;
			random_vector = Ogre::Vector3((static_cast<float>(rand()) / RAND_MAX) * ceil,
				(static_cast<float>(rand()) / RAND_MAX) * ceil,
				(static_cast<float>(rand()) / RAND_MAX) * ceil);
		}
		element.position = step.normalisedCopy().crossProduct(random_vector) + element_position;
		mBillboardChain->updateChainElement(0,i,element);
		element_position += step;
	}

	return true;
}