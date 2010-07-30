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
		HWND							GetWindowHandle();
		
		// ��ô��ڴ�С
		KVOID							GetWindowSize(KUINT&,KUINT&);
		
		// �ж��Ƿ���ĳ��
		KBOOL							IsKeyDown(KUINT uKeyCode);
		
		// �Ƿ��������
		KBOOL							IsOpenConsole();

		// �Ƿ���ͣ
		KBOOL							IsPaused();
		// ��ͣ
		KVOID							Pause();
		// �ָ�
		KVOID							Resume();
		// �˳�
		KVOID							ShutDown();
		// ����
		KVOID							ScreenShot();
		//�����Դ��Ŀ¼
		KSTR							GetRootMedia();
		//������Դ
		KVOID							LoadResource(KSTR sRes);
		//��ýŲ�ָ��
		ScriptVM*						GetScriptVM();
		// ������״̬
		OIS::MouseState					GetMouseState();
		// ��õ�ǰ֡״̬
		Ogre::RenderTarget::FrameStats	GetSystemStats();
		
		// �������ֻ�������
		Ogre::Camera*					GetCamera(KCCHAR* pName);
		// ��������������ָ��
		CameraControl*					GetCameraController();
		// ���GUI����ָ��
		GuiManager*						GetGuiManager();
		//
		InputManager*					GetInputManager();
		// ��ó�������ָ��
		Ogre::SceneManager*				GetSceneManager();
		//
		RenderableManager*				GetRenderableManager();
		//
		Ogre::RenderWindow*				GetMainWindow();
		// ����������
		KBOOL							GetMouseRay(KPoint2 vOrg, Ogre::Ray &kRay);
		KBOOL							GetMouseRay(KPoint2 vOrg, Ogre::Ray &kRay, Ogre::Camera* pCamera);

		// ���������
		Ogre::Camera*					CreateCamera(KCCHAR* pName);
		// �����Ӵ�
		Ogre::Viewport*					CreateViewports(Ogre::Camera* pCamera, KColor kValue = KColor(0.2f,0.3f,0.4f));
		// ��������
		Ogre::TerrainGroup*				CreateTerrainGroup(KINT nMapSize, KFLOAT fWorldSize);

		// ������������
		KVOID							CreateSceneQuery();

		inline Ogre::RaySceneQuery*		GetRayQuery() { return m_pRaySceneQuery; }
		inline Ogre::SphereSceneQuery*	GetSphereQuery() { return m_pSphereSceneQuery; }

		// ���ٳ�������
		KVOID							DestroyQuery();
		// 
		CameraControl*					CreateCameraControl(Ogre::Camera* pCamera);
		//
		KVOID							DestroyCameraControl();

		// ע�����
		KVOID							RegOptListener(InputListener* pl);
		// ж�ؼ���
		KVOID							UnregOptListener(InputListener* pl);
		
		// ����ͼ��
		KVOID							SetWindowIcon(KCCHAR* pIcon);

	protected:
		// ������ߺͳ�������
		Ogre::RaySceneQuery*	m_pRaySceneQuery;
		Ogre::SphereSceneQuery*	m_pSphereSceneQuery;
	};
}

#define GET_GUI_PTR(class_name) \
	static_cast<class_name*>(Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase(#class_name))