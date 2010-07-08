#pragma once

//#include "CCSCameraControlSystem.h"

namespace Kylin
{
	class CameraControl
	{
	public:
		CameraControl();
		~CameraControl();

		enum Mode
		{
			CC_FREE,
			CC_FIRST,
			CC_THIRD,
		};

		KBOOL Initialize(Ogre::SceneManager* pSceneMgr, Ogre::Camera* pCamera, KSTR sName="kylin_camera_control");

		KVOID SetTarget(Ogre::SceneNode* pNode);
		KVOID SetMode(KSTR sMode);
		//CCS::CameraControlSystem::CameraMode* GetMode(KSTR sMode);

		KVOID Update(KFLOAT fElapsed);

		Ogre::Camera* GetActiveCamera();

	protected:
		//CCS::CameraControlSystem* m_pCameraCS;

	};
}