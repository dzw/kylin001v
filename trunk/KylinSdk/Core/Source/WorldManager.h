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
			WS_NOT_ENTER,				//û�н����κγ���
			WS_ENTER_ING,				//���ڽ���һ������(�Ѿ�������CGEnterScene)
			WS_ENTER,					//�ɹ�����һ������
			WS_RELIVE,					//��������
			WS_ASK_SWITCH_SCANE,		//Ҫ���л���һ���µĳ���(�Ѿ�������CGAskSwitchScene)
		};	
		
		virtual KBOOL			Initialize(KCCHAR* pWorldCfg);
		virtual KVOID			Destroy();

		/**
			����ĳ����
			\param nSceneID	
				����ID/��ҳ���id

			\param nCityLevel
				��ҳ��еȼ������С��0����ʾ����ͨ����!
		*/
		virtual KBOOL			EnterScene(KINT nSceneID);
		// ��Ϸ�������л�����
		virtual KVOID			SwitchScene(KINT idTargetScene);
		// ȡ�õ�ǰ״̬
		virtual WORLD_STATION	GetStation(KVOID) const	{ return m_eStation; }
		// ȡ�õ�ǰ����ĳ���
		virtual Scene*			GetActiveScene(KVOID);
		//
		virtual KVOID			Tick(KFLOAT fElapsed);
		
	private:
		friend class KylinRoot;
		
		// ������Ϣ
		KMAP<KUINT,SceneHag*>	m_kSceneInfo;
		//��ǰ����ĳ���
		Scene*					m_pActiveScene;
		//��ǰ״̬
		WORLD_STATION			m_eStation;
	};
}
