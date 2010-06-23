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
		// 开始产生Monster
		virtual KVOID	Accept(float fInterval, float fDelay, int nMaxCount, KUINT uSpawnID);
		virtual KVOID	DoSpawn();

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

		//KVEC<KUINT>	m_kChildList;
	};
}