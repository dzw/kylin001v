#pragma once


namespace Kylin
{

	class WorldManager
	{
	public:
		WorldManager();
		virtual ~WorldManager();

		enum WORLD_STATION
		{
			WS_NOT_ENTER,				//没有进入任何场景
			WS_ENTER_ING,				//正在进入一个场景(已经发送了CGEnterScene)
			WS_ENTER,					//成功进入一个场景
			WS_RELIVE,					//死亡复活
			WS_ASK_CHANGE_SCANE,		//要求切换到一个新的场景(已经发送了CGAskChangeScene)
		};	
		
		virtual KBOOL			Initialize(KCCHAR* pWorldCfg);
		virtual KVOID			Destroy();

		/**
			进入某场景
			\param nSceneID	
				场景ID/玩家城市id

			\param nCityLevel
				玩家城市等级，如果小于0，表示是普通场景!
		*/
		virtual KBOOL			EnterScene(KINT nSceneID);
		// 游戏过程中切换场景
		virtual KVOID			ChangeScene(KINT idTargetScene, const KPoint2& fvPos, KINT nDirection, BYTE bFlag);
		// 取得当前状态
		virtual WORLD_STATION	GetStation(KVOID) const	{ return m_eStation; }
		// 取得当前激活的场景
		virtual Scene*			GetActiveScene(KVOID);
		//
		virtual KVOID			Tick(KFLOAT fElapsed);
		
	private:
		friend class KylinRoot;
		
		// 场景信息
		KMAP<KUINT,SceneHag*>	m_kSceneInfo;
		//当前激活的场景
		Scene*					m_pActiveScene;
		//当前状态
		WORLD_STATION			m_eStation;
	};
}
