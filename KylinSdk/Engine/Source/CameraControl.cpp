#include "engpch.h"
#include "CameraControl.h"
#include "CCSBasicCameraModes.h"
#include "CCSFreeCameraMode.h"
#include "rOgreRoot.h"


Kylin::CameraControl::CameraControl()
: m_pCameraCS(NULL)
{

}

KBOOL Kylin::CameraControl::Initialize( SceneManager* pSceneMgr, Camera* pCamera, KSTR sName/*="kylin_camera_control"*/ )
{
	
	m_pCameraCS = new CCS::CameraControlSystem(pSceneMgr, sName, pCamera);

	CCS::FirstPersonCameraMode* pCamMode = new CCS::FirstPersonCameraMode(m_pCameraCS,Ogre::Vector3(0,17,-16)
		, Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
	m_pCameraCS->registerCameraMode("FirstPerson",pCamMode);

	CCS::ChaseFreeYawAxisCameraMode* pCamModeC = new CCS::ChaseFreeYawAxisCameraMode(m_pCameraCS,Ogre::Vector3(0,15,-22)
		, Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(Ogre::Degree(-18)));
	m_pCameraCS->registerCameraMode("ChasePerson",pCamModeC);

	CCS::FreeCameraMode* pCamModeF = new CCS::FreeCameraMode(m_pCameraCS);
	m_pCameraCS->registerCameraMode("Free",pCamModeF);
	pCamModeF->setMoveFactor(10);
	
	m_pCameraCS->setCurrentCameraMode(pCamModeF);
	
	return true;
}

KVOID Kylin::CameraControl::SetTarget( Ogre::SceneNode* pNode )
{
	// Set the camera target
	m_pCameraCS->setCameraTarget(pNode);
}

KVOID Kylin::CameraControl::SetMode( KSTR sMode )
{
	CCS::CameraControlSystem::CameraMode* pMode = m_pCameraCS->getCameraMode(sMode);
	if (pMode)
		m_pCameraCS->setCurrentCameraMode(pMode);
}

KVOID Kylin::CameraControl::Update( KFLOAT fElapsed )
{
	if (OgreRoot::GetSingletonPtr()->IsOpenConsole())
		return;

	m_pCameraCS->update(fElapsed);

	if(m_pCameraCS->getCameraModeName(m_pCameraCS->getCurrentCameraMode()) == "Free")
	{
		CCS::FreeCameraMode* freeCameraMode = (CCS::FreeCameraMode*)m_pCameraCS->getCameraMode("Free");

		if(OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_A))
			freeCameraMode->goLeft();

		if(OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_D))
			freeCameraMode->goRight();

		if( OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_UP) 
			|| OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_W) )
			freeCameraMode->goForward();

		if( OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_DOWN) 
			|| OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_S) )
			freeCameraMode->goBackward();

		if(OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_E))
			freeCameraMode->goUp();

		if(OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_Q))
			freeCameraMode->goDown();

		if(OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_RIGHT))
			freeCameraMode->yaw(-1);

		if(OgreRoot::GetSingletonPtr()->IsKeyDown(OIS::KC_LEFT))
			freeCameraMode->yaw(1);

		const OIS::MouseState &ms = OgreRoot::GetSingletonPtr()->GetMouseState();
		if( ms.buttonDown( OIS::MB_Right ) )
		{
			freeCameraMode->yaw(ms.X.rel);
			freeCameraMode->pitch(ms.Y.rel);
		}
	}
}

Camera* Kylin::CameraControl::GetActiveCamera()
{
	return m_pCameraCS->getOgreCamera();
}

CCS::CameraControlSystem::CameraMode* Kylin::CameraControl::GetMode( KSTR sMode )
{
	return m_pCameraCS->getCameraMode(sMode);
}