#pragma once

#include "Character.h"

namespace Kylin
{
	class ObjectSpawner : public Character
	{
	public:
		BtDeclareRTTI
		Declare_Event_Handler(ObjectSpawner, Entity);
		Declare_Entity_Creator(ObjectSpawner)

		ObjectSpawner();
		
		//virtual KVOID	Tick(KFLOAT fElapsed);

		//-----------------------------------------------------------------
		// ��ʼ����Monster��ÿ����һ��monster��m_nLevel�Զ���һ
		// fInterval	spawner ���ɺ��ÿ�ʼ����monster
		// fDelay		�Ⱥ�����monster �Ĳ���ʱ����
		// nMaxCount	����������ٸ�monster
		// uSpawnID		monster �� gid
		// bAllDead		�����в������� monster ������ſɲ�����һ��monster�ı��
		virtual KVOID	Accept(KFLOAT fInterval, KFLOAT fDelay, KINT nMaxCount, KUINT uSpawnID,KBOOL bAllDead = false);

		virtual KVOID	DoSpawn();
	
	protected:
		virtual KBOOL	CheckAllDead();

	protected:
		KVOID			EV_DoSpawn(EventPtr spEV);
		
		//-----------------------------------------------------------------
		
	protected:
		
		KINT		m_nLevel;
		KUINT		m_uSpawnID;
		KFLOAT		m_fSpawnInterval;
		KFLOAT		m_fSpawnDelay;
		KFLOAT		m_fTempTime;
		KINT		m_nMaxCount;
		
		KBOOL		m_bAllDead;
		KVEC<KUINT>	m_kChildList;
	};
}