#pragma once

namespace Kylin
{
	class Lightning : public EffectObject
	{
	public:
		Lightning(Ogre::SceneManager* scene_manager);
		~Lightning();

		void AddToScene(const Ogre::Vector3& position = Ogre::Vector3::ZERO ,
			const Ogre::Quaternion& orientation  = Ogre::Quaternion::IDENTITY);

		void AddToScene(Ogre::SceneNode *start_node, Ogre::SceneNode *end_node);

		void RemoveFromScene();
		bool UpdateLogic(Ogre::Real time_step);

	private:
		Ogre::BillboardChain *mBillboardChain;

		Ogre::SceneNode *mStartNode;
		Ogre::SceneNode *mEndNode;

		Ogre::Vector3 mStartPosition;
		Ogre::Vector3 mEndPosition;
		Ogre::Vector3 mDistance;

	};
}