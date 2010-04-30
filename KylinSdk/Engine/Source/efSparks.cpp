#include "engpch.h"
#include "efSparks.h"

Kylin::Sparks::Sparks(Ogre::SceneManager* scene_manager)
{
	mTimeToLive = 0;
	mEmitter = 0;
}

Kylin::Sparks::~Sparks()
{

}

void Kylin::Sparks::AddToScene(const Ogre::Vector3& position , const Ogre::Quaternion& orientation)
{
	// if particle system does not exist nothing will happen
	bool result = mSceneManager->hasParticleSystem("Spark");
	if (result)
	{
		Ogre::ParticleSystem* mParticleSystem = mSceneManager->getParticleSystem("Spark");

		// set up emitter
		mEmitter = mParticleSystem->addEmitter("Point");
		mEmitter->setPosition(position);
		mTimeToLive = 0.3;
		mEmitter->setTimeToLive(mTimeToLive);
		mEmitter->setDuration(0.1);
		//		mEmitter->setParticleVelocity(13,23);
		//		mEmitter->setEmissionRate(240);
		mEmitter->setAngle(Ogre::Degree(20));
		mEmitter->setEnabled(true);
	}
}

void Kylin::Sparks::RemoveFromScene()
{
	if (mEmitter)
		mEmitter->setEnabled(false);
}

bool Kylin::Sparks::UpdateLogic(Ogre::Real time_step)
{
	mTimeToLive -= time_step;

	// if particle system expires remove and delete
	if (mTimeToLive <= 0)
		return false;

	return true;
}