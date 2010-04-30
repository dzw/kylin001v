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
		HWND  GetWindowHandle();
		
		// 获得窗口大小
		KVOID GetWindowSize(KUINT&,KUINT&);
		
		// 判断是否按下某键
		KBOOL IsKeyDown(KUINT uKeyCode);
		
		// 是否可以输入
		KBOOL IsOpenConsole();

		// 是否暂停
		KBOOL IsPaused();
		// 暂停
		KVOID Pause();
		// 恢复
		KVOID Resume();
		// 退出
		KVOID ShutDown();
		// 截屏
		KVOID ScreenShot();
		
		//获得脚步指针
		ScriptVM*			GetScriptVM();
		// 获得鼠标状态
		OIS::MouseState		GetMouseState();
		// 获得当前帧状态
		Ogre::RenderTarget::FrameStats GetSystemStats();
		
		// 根据名字获得摄像机
		Ogre::Camera*		GetCamera(KCCHAR* pName);
		// 获得摄像机控制类指针
		CameraControl*		GetCameraController();
		// 获得GUI管理指针
		GuiManager*			GetGuiManager();
		// 获得场景管理指针
		Ogre::SceneManager*	GetSceneManager();
		// 获得鼠标射线
		KBOOL				GetMouseRay(KPoint2 vOrg, Ogre::Ray &kRay);

		// 创建摄像机
		Ogre::Camera*		CreateCamera(KCCHAR* pName);
		// 创建视窗
		Ogre::Viewport*		CreateViewports(Ogre::Camera* pCamera, KColor kValue = KColor(0.2f,0.3f,0.4f));
		// 创建地形
		Ogre::TerrainGroup* CreateTerrainGroup(KINT nMapSize, KFLOAT fWorldSize);
		

		// 注册监听
		KVOID RegOptListener(InputListener* pl);
		// 卸载监听
		KVOID UnregOptListener(InputListener* pl);
	};
}