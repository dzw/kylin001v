#pragma once

namespace Kylin
{
	struct SceneHag 
	{
		SceneHag();

		// ����ID
		KUINT	m_uSceneID;
		// ��������
		KUINT	m_uType;
		// ��������
		KUINT	m_uLevel;
		// ��������
		KSTR	m_sName;
		// ��������
		KINT	m_nBgSound;
		// �����ļ�
		KSTR	m_sSceneFile;
		// �Ƿ�ͨ��
		KBOOL	m_bPass;
	};

	class Scene
	{
	public:
		Scene(const SceneHag& kSceneHag);
		~Scene();	

		virtual KVOID	Tick(KFLOAT fElapsed);
		//���볡��
		virtual KVOID	EnterScene(KVOID);
		//�뿪����
		virtual KVOID	LeaveScene(KVOID);
		//����λ���Ƿ�Ϸ�
		virtual KBOOL	IsValidPosition(const KPoint2& fvPos);
		//��ó������ؽӿ�
		SceneLoader*	GetSceneLoader();
		
	protected:
		virtual KVOID	SpawnScene();

		//������Ч�б�
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
		
		//��������
		SceneHag		m_kSceneHag;
		//
		//�������ؽӿ�
		SceneLoader*	m_pSceneLoader;
		//����ENTITY������
		EntityManager*	m_pEntityManager;
		//�¼�������
		EventManager*	m_pEventManager;
	};
}
