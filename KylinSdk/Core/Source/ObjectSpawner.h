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
		// 开始产生Monster，每产生一批monster后，m_nLevel自动加一
		// fInterval	spawner 生成后多久开始产生monster
		// fDelay		先后两个monster 的产生时间间隔
		// nMaxCount	最多会产生多少个monster
		// uSpawnID		monster 的 gid
		// bAllDead		当所有产生出的 monster 死亡后才可产生下一批monster的标记
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