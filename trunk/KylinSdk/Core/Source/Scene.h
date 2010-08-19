#pragma once

namespace Kylin
{
	struct SceneHag 
	{
		SceneHag();

		// 场景ID
		KUINT	m_uSceneID;
		// 场景类型
		KUINT	m_uType;
		// 场景级别
		KUINT	m_uLevel;
		// 场景名称
		KSTR	m_sName;
		// 背景音乐
		KINT	m_nBgSound;
		// 场景文件
		KSTR	m_sSceneFile;
		// 是否通过
		KBOOL	m_bPass;
	};

	class Scene
	{
	public:
		Scene(const SceneHag& kSceneHag);
		~Scene();	

		virtual KVOID	Tick(KFLOAT fElapsed);
		//进入场景
		virtual KVOID	EnterScene(KVOID);
		//离开场景
		virtual KVOID	LeaveScene(KVOID);
		//场景位置是否合法
		virtual KBOOL	IsValidPosition(const KPoint2& fvPos);
		//获得场景加载接口
		SceneLoader*	GetSceneLoader();
		
	protected:
		virtual KVOID	SpawnScene();

		//环境音效列表
		struct EnvSound_t
		{
			KINT			nID;
			KINT			nSoundID;
			KUINT			nPosx;
			KUINT			nPosz;
			KUINT			nDistance;
			//tSoundSource*	pSoundSource;
		};

		KVEC<EnvSound_t>	m_kEnvSoundList;

	private:
		friend class KylinRoot;
		
		//场景数据
		SceneHag		m_kSceneHag;
		//
		//场景加载接口
		SceneLoader*	m_pSceneLoader;
		//场景ENTITY管理器
		EntityManager*	m_pEntityManager;
		//事件管理器
		EventManager*	m_pEventManager;
	};
}
