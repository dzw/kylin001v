#pragma once

#include "Singleton.h"

namespace Kylin
{
	class OgreRoot : public Singleton<OgreRoot>
	{
	public:
		OgreRoot();
		virtual ~OgreRoot();
		
		// ��ô��ھ��
		HWND  GetWindowHandle();
		
		// ��ô��ڴ�С
		KVOID GetWindowSize(KUINT&,KUINT&);
		
		// �ж��Ƿ���ĳ��
		KBOOL IsKeyDown(KUINT uKeyCode);
		
		// �Ƿ��������
		KBOOL IsOpenConsole();

		// �Ƿ���ͣ
		KBOOL IsPaused();
		// ��ͣ
		KVOID Pause();
		// �ָ�
		KVOID Resume();
		// �˳�
		KVOID ShutDown();
		// ����
		KVOID ScreenShot();
		//�����Դ��Ŀ¼
		KSTR GetRootMedia();
		//������Դ
		KVOID LoadResource(KSTR sRes);
		//��ýŲ�ָ��
		ScriptVM*			GetScriptVM();
		// ������״̬
		OIS::MouseState		GetMouseState();
		// ��õ�ǰ֡״̬
		Ogre::RenderTarget::FrameStats GetSystemStats();
		
		// �������ֻ�������
		Ogre::Camera*		GetCamera(KCCHAR* pName);
		// ��������������ָ��
		CameraControl*		GetCameraController();
		// ���GUI����ָ��
		GuiManager*			GetGuiManager();
		// ��ó�������ָ��
		Ogre::SceneManager*	GetSceneManager();
		//
		Ogre::RenderWindow*	GetMainWindow();
		// ����������
		KBOOL				GetMouseRay(KPoint2 vOrg, Ogre::Ray &kRay);
		KBOOL				GetMouseRay(KPoint2 vOrg, Ogre::Ray &kRay, Ogre::Camera* pCamera);
		//
		KBOOL				PickOgreEntity(Ogre::Ray &rRay, Ogre::Entity **ppResult, KUINT uQueryMask = 0xffffffff);
		// ���������
		Ogre::Camera*		CreateCamera(KCCHAR* pName);
		// �����Ӵ�
		Ogre::Viewport*		CreateViewports(Ogre::Camera* pCamera, KColor kValue = KColor(0.2f,0.3f,0.4f));
		// ��������
		Ogre::TerrainGroup* CreateTerrainGroup(KINT nMapSize, KFLOAT fWorldSize);
		// ������������
		Ogre::RaySceneQuery* CreateSceneRay();
		//
		KVOID				DestroySceneRay();
		// 
		CameraControl*		CreateCameraControl(Ogre::Camera* pCamera);
		//
		KVOID				DestroyCameraControl();

		// ע�����
		KVOID RegOptListener(InputListener* pl);
		// ж�ؼ���
		KVOID UnregOptListener(InputListener* pl);

	protected:
		Ogre::RaySceneQuery* m_pRaySceneQuery;
	};
}