#pragma once

#include "Singleton.h"

namespace Kylin
{
	class OgreRoot : public Singleton<OgreRoot>
	{
	public:
		OgreRoot();
		virtual ~OgreRoot();
		
		// 获得窗口句柄
		HWND							GetWindowHandle();
		
		// 获得窗口大小
		KVOID							GetWindowSize(KUINT&,KUINT&);
		
		// 判断是否按下某键
		KBOOL							IsKeyDown(KUINT uKeyCode);
		
		// 是否可以输入
		KBOOL							IsOpenConsole();

		// 是否暂停
		KBOOL							IsPaused();
		// 暂停
		KVOID							Pause();
		// 恢复
		KVOID							Resume();
		// 退出
		KVOID							ShutDown();
		// 截屏
		KVOID							ScreenShot();
		//获得资源跟目录
		KSTR							GetRootMedia();
		//加载资源
		KVOID							LoadResource(KSTR sRes);
		//获得脚步指针
		ScriptVM*						GetScriptVM();
		// 获得鼠标状态
		OIS::MouseState					GetMouseState();
		// 获得当前帧状态
		Ogre::RenderTarget::FrameStats	GetSystemStats();
		
		// 根据名字获得摄像机
		Ogre::Camera*					GetCamera(KCCHAR* pName);
		// 获得摄像机控制类指针
		CameraControl*					GetCameraController();
		// 获得GUI管理指针
		GuiManager*						GetGuiManager();
		//
		InputManager*					GetInputManager();
		// 获得场景管理指针
		Ogre::SceneManager*				GetSceneManager();
		//
		Ogre::RenderWindow*				GetMainWindow();
		// 获得鼠标射线
		KBOOL							GetMouseRay(KPoint2 vOrg, Ogre::Ray &kRay);
		KBOOL							GetMouseRay(KPoint2 vOrg, Ogre::Ray &kRay, Ogre::Camera* pCamera);

		// 创建摄像机
		Ogre::Camera*					CreateCamera(KCCHAR* pName);
		// 创建视窗
		Ogre::Viewport*					CreateViewports(Ogre::Camera* pCamera, KColor kValue = KColor(0.2f,0.3f,0.4f));
		// 创建地形
		Ogre::TerrainGroup*				CreateTerrainGroup(KINT nMapSize, KFLOAT fWorldSize);

		// 创建场景射线
		Ogre::RaySceneQuery*			CreateRaySceneQuery();
		Ogre::SphereSceneQuery*			CreateSphereSceneQuery();

		inline Ogre::RaySceneQuery*		GetRayQuery() { return m_pRaySceneQuery; }
		inline Ogre::SphereSceneQuery*	GetSphereQuery() { return m_pSphereSceneQuery; }

		// 销毁场景射线
		KVOID							DestroyQuery();
		// 
		CameraControl*					CreateCameraControl(Ogre::Camera* pCamera);
		//
		KVOID							DestroyCameraControl();

		// 注册监听
		KVOID							RegOptListener(InputListener* pl);
		// 卸载监听
		KVOID							UnregOptListener(InputListener* pl);

	protected:
		// 鼠标射线和场景交集
		Ogre::RaySceneQuery*	m_pRaySceneQuery;
		Ogre::SphereSceneQuery*	m_pSphereSceneQuery;
	};
}

#define GET_GUI_PTR(class_name) \
	static_cast<class_name*>(Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase(#class_name))