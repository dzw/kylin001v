#pragma once

namespace CCS
{
	class CameraControlSystem;
}

namespace Kylin
{
	class CameraControl
	{
	public:
		CameraControl();
		
		enum Mode
		{
			CC_FREE,
			CC_FIRST,
			CC_THIRD,
		};

		KBOOL Initialize(SceneManager* pSceneMgr, Camera* pCamera, KSTR sName="kylin_camera_control");

		KVOID SetTarget(Ogre::SceneNode* pNode);
		KVOID SetMode(KSTR sMode);
		
		KVOID Update(KFLOAT fElapsed);

		Camera* GetActiveCamera();

	protected:
		CCS::CameraControlSystem* m_pCameraCS;

	};
}