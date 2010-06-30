#include "corepch.h"
#include "GameCamera.h"

#define CAMERA_DIS 10.0f
#define MAX_CAM_DIS 35
#define MIN_CAM_DIS 10

Kylin::GameCamera::GameCamera( Ogre::Camera* pCam,Ogre::SceneManager* pSceneMgr )
: m_pCamera(pCam)
, m_pSceneMnger(pSceneMgr)
, m_pTargetNode(NULL)
, m_pCameraPivot(NULL)
, m_pCameraGoal(NULL)
, m_pCameraNode(NULL)
, m_fCamPosY(0.8f)
, m_fCameraDistance(CAMERA_DIS)
, m_eMode(CM_NULL)
{

}

KVOID Kylin::GameCamera::SetTarget( Ogre::SceneNode* pTargetNode )
{
	m_pTargetNode = pTargetNode;
	KPoint3	kPos  = m_pTargetNode->getPosition();
	//------------
	m_pCameraPivot = m_pSceneMnger->getRootSceneNode()->createChildSceneNode(/*pos*/);
	m_pCameraPivot->setPosition(kPos + KPoint3::UNIT_Y * CAM_HEIGHT);
	// this is where the camera should be soon, and it spins around the pivot
	m_pCameraGoal = m_pCameraPivot->createChildSceneNode(KPoint3(0, 0, m_fCameraDistance));
	// this is where the camera actually is

	m_pCameraNode = m_pSceneMnger->getRootSceneNode()->createChildSceneNode();
	m_pCameraNode->setPosition(m_pCameraPivot->getPosition() + m_pCameraGoal->getPosition());

	m_pCameraPivot->setFixedYawAxis(true);
	m_pCameraGoal->setFixedYawAxis(true);
	m_pCameraNode->setFixedYawAxis(true);

	m_pCamera->setFixedYawAxis(true);
	m_pCamera->setPosition(m_pCameraNode->getPosition());
	m_fPivotPitch = 0;

	// test code
	m_eMode = CM_3P;
}

KVOID Kylin::GameCamera::Update( KFLOAT fElapsed )
{
	if (m_eMode == CM_3P)
	{
		KPoint3 Capos = m_pCameraNode->getPosition();
		KPoint3 Nopos = m_pTargetNode->getPosition();
		// place the camera pivot roughly at the character's shoulder
		m_pCameraPivot->setPosition(Nopos + KPoint3::UNIT_Y * CAM_HEIGHT);
		// move the camera smoothly to the goal
		KPoint3 goalOffset = m_pCameraGoal->_getDerivedPosition() - m_pCameraNode->getPosition();

		KPoint3 DistPos = Capos + goalOffset; //* fElapsed * 9.0f;
		if(DistPos.y < GetCameraPosY())
			DistPos.y = GetCameraPosY();
		m_pCameraNode->setPosition(DistPos);
		// always look at the pivot
		m_pCameraNode->lookAt(m_pCameraPivot->_getDerivedPosition(), Ogre::Node::TS_WORLD);
		m_pCamera->setPosition(DistPos);
		m_pCamera->lookAt(m_pCameraPivot->_getDerivedPosition());
	}
}

KVOID Kylin::GameCamera::Destroy()
{
	if (m_pCameraNode)
		m_pSceneMnger->destroySceneNode(m_pCameraNode);
	if (m_pCameraPivot)
		m_pSceneMnger->destroySceneNode(m_pCameraPivot);

	m_pCameraNode  = NULL;
	m_pCameraPivot = NULL;
}


KVOID Kylin::GameCamera::UpdateCameraGoal( KFLOAT fDeltaYaw, KFLOAT fDeltaPitch, KFLOAT fDeltaZoom )
{
	m_pCameraPivot->yaw(Ogre::Degree(fDeltaYaw), Ogre::Node::TS_WORLD);

	// bound the pitch
	if (!(m_fPivotPitch + fDeltaPitch > 25 && fDeltaPitch > 0) &&
		!(m_fPivotPitch + fDeltaPitch < -60 && fDeltaPitch < 0))
	{
		//Ogre::Radian temp = Ogre::Degree(fDeltaPitch);
		m_pCameraPivot->pitch(Ogre::Degree(fDeltaPitch), Ogre::Node::TS_LOCAL);
		m_fPivotPitch += fDeltaPitch;
	}

	KFLOAT dist = GetCameraDistance();
	KFLOAT distChange = fDeltaZoom * dist;

	if(dist + distChange > MIN_CAM_DIS && dist + distChange < MAX_CAM_DIS)
	{
		ChangeCameraDistance(distChange);
	}
}

//////////////////////////////////////////////////////////////////////////
KVOID Kylin::GameCamera::SetCameraPosition( const KPoint3& pos )
{
	m_pCameraNode->setPosition(pos);
	m_pCamera->setPosition(pos);
}

const KPoint3& Kylin::GameCamera::GetCameraPosition()
{
	return m_pCameraNode->getPosition();
}

const KPoint3& Kylin::GameCamera::GetCameraDirection()
{
	return m_pCameraNode->getOrientation() * KPoint3::UNIT_Z;
}

KFLOAT Kylin::GameCamera::GetCameraDistance()
{
	return m_fCameraDistance;
}

KVOID Kylin::GameCamera::SetCameraDistance( KFLOAT fDist )
{
	m_fCameraDistance = fDist;
}

KVOID Kylin::GameCamera::ChangeCameraDistance( KFLOAT fDist )
{
	KFLOAT fCurDis = GetCameraDistance();
	if (fCurDis + fDist > 0 && fCurDis + fDist < MAX_CAM_DIS)
	{
		SetCameraDistance(fCurDis + fDist);
		m_pCameraGoal->translate(0, 0, fDist, Ogre::Node::TS_LOCAL);
	}
}

KVOID Kylin::GameCamera::SetCameraPosY( KFLOAT fHight )
{
	m_fCamPosY = fHight;
}

KFLOAT Kylin::GameCamera::GetCameraPosY()
{
	return m_fCamPosY;
}

Ogre::SceneNode* Kylin::GameCamera::GetCameraNode()
{
	return m_pCameraNode;
}

KVOID Kylin::GameCamera::SetMode( GameCameraMode eMod )
{
	if (eMod == CM_FREE)
	{
		m_kOldPos = m_pCamera->getPosition();
	}
	else if (eMod == CM_3P)
	{
		if (m_eMode == CM_FREE)
		{
			m_pCamera->setPosition(m_kOldPos);
		}
	}

	m_eMode = eMod;
}

Ogre::Ray Kylin::GameCamera::GetCameraToTargetRay()
{
	KPoint3 kDir = m_pTargetNode->getPosition()-m_pCamera->getPosition();
	kDir.normalise();
	Ogre::Ray kRay = Ogre::Ray(m_pCamera->getPosition(),kDir);
	
	
	return kRay;
}