#pragma once

namespace Kylin
{
	class Sparks : public EffectObject
	{
	public:
		Sparks(Ogre::SceneManager* scene_manager);
		~Sparks();

		void AddToScene(const Ogre::Vector3& position = Ogre::Vector3::ZERO ,
			const Ogre::Quaternion& orientation  = Ogre::Quaternion::IDENTITY);
		void RemoveFromScene();

		Ogre::ParticleEmitter* GetEmitter() const
		{
			return mEmitter;
		}

		bool UpdateLogic(Ogre::Real time_step);

	private:
		Ogre::ParticleEmitter *mEmitter;
		Ogre::ParticleSystem *mParticleSystem;
		Ogre::Real mTimeToLive;
	};

}