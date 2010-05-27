#pragma once

#define CAM_HEIGHT 3           // height of camera above character's center of mass

namespace Kylin
{
	class GameCamera
	{
	public:
		GameCamera(Ogre::Camera* pCam,Ogre::SceneManager* pSceneMgr);
		
		virtual KVOID SetTarget(Ogre::SceneNode* pTargetNode);
		virtual KVOID Update(KFLOAT fElapsed);
		virtual KVOID Destroy();

		enum GameCameraMode 
		{
			CM_NULL,
			CM_1P,
			CM_3P,
			CM_FREE
		};

	public:
		KVOID			SetCameraPosition(const KPoint3& pos);
		const KPoint3&	GetCameraPosition();
		const KPoint3&	GetCameraDirection();
		
		// 改变摄像机与主角间距离（当摄像机与主角之间有地形高出时）
		KFLOAT			GetCameraDistance();
		KVOID			SetCameraDistance(KFLOAT fDist);
		KVOID			ChangeCameraDistance(KFLOAT fDist);

		// 控制摄像机贴地面时的操作的
		KVOID			SetCameraPosY(KFLOAT fHight);
		KFLOAT			GetCameraPosY();
		
		Ogre::SceneNode* GetCameraNode();
		
		// 设置摄像机模式
		KVOID			SetMode(GameCameraMode eMod){ m_eMode = eMod; }
		GameCameraMode	GetMode() { return m_eMode; }

	public:
		virtual KVOID UpdateCameraGoal(KFLOAT fDeltaYaw, KFLOAT fDeltaPitch, KFLOAT fDeltaZoom);
				
	protected:
		Ogre::SceneManager*		m_pSceneMnger;
		Ogre::Camera*			m_pCamera;
		Ogre::SceneNode*		m_pTargetNode;
		Ogre::SceneNode*		m_pCameraPivot;
		Ogre::SceneNode*		m_pCameraGoal;
		Ogre::SceneNode*		m_pCameraNode;
		
		KFLOAT			m_fPivotPitch;
		KFLOAT			m_fCameraDistance;

		KFLOAT			m_fCamPosY;

		GameCameraMode	m_eMode;
	};
}