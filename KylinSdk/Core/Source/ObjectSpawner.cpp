#include "corepch.h"
#include "ObjectSpawner.h"
#include "RegisterClass.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"
#include "rOgreRoot.h"
#include "ScriptVM.h"


namespace Kylin
{

	BtImplementRTTI(ObjectSpawner, Character, id_spawner);

	Implement_Event_Handler(ObjectSpawner, Character)
	{
		{&ev_do_spawn,				&EV_DoSpawn},
		{NULL, NULL}
	};

	ObjectSpawner::ObjectSpawner()
		: m_uSpawnID(INVALID_ID)
		, m_fSpawnInterval(-1)
		, m_fSpawnDelay(-1)
		, m_nMaxCount(0)
		, m_fTempTime(.0f)
		, m_nLevel(1)
	{

	}

// 	KVOID ObjectSpawner::Tick( KFLOAT fElapsed )
// 	{
// 
// 	}

	KVOID ObjectSpawner::Accept( float fInterval, float fDelay, int nMaxCount, KUINT uSpawnID, KBOOL bAllDead /*= false*/ )
	{
		m_fSpawnInterval	= fInterval;
		m_fSpawnDelay		= fDelay;
		m_nMaxCount			= nMaxCount;
		m_uSpawnID			= uSpawnID;

		m_fTempTime			= m_fSpawnInterval;
		m_bAllDead			= bAllDead;
		//-----------------------------------------------------------
		DoSpawn();

		m_kChildList.clear();
	}

	//---------------------------------------------------------------
	KVOID ObjectSpawner::DoSpawn()
	{
		Assert(m_uSpawnID != INVALID_ID);
		Assert(m_fSpawnInterval > 0);
		Assert(m_fSpawnDelay > 0);

		if (m_nMaxCount <= 0)
			return;
		m_nMaxCount--;

		EventPtr spEV(
			KNEW Event(
			&ev_do_spawn, 
			Event::ev_timing, 
			m_fTempTime,
			0, 
			NULL
			));

		KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);
	}

	KVOID ObjectSpawner::EV_DoSpawn( EventPtr spEV )
	{
		//-----------------------------------------------------------------
		// 
		m_fTempTime	= m_fSpawnDelay;
		DoSpawn();

		if (m_bAllDead && !CheckAllDead())
			return;

		//-----------------------------------------------------------------
		Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->SpawnCharactor(m_uSpawnID);
		if (pEnt)
		{
			pEnt->SetTranslate(this->GetTranslate());
			pEnt->SetRotation(this->GetRotation());

			m_kChildList.push_back(pEnt->GetID());

			//-----------------------------------------------------------------
			// 回调脚步计时器
			KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_timer");
			//-----------------------------------------------------------------
			if (m_nMaxCount <= 0)
			{
				m_nLevel++;
				//-----------------------------------------------------------------
				// 完成一组NPC后调用脚步
				KUINT uGID = -1;
				if ( this->GetPropertyRef().GetUIntValue("$GID",uGID) )
				{
					KSTR sModule = "char_";
					sModule += Ogre::StringConverter::toString(uGID);

					KVEC<KCCHAR *> kModules;
					kModules.push_back(sModule.data());

					OgreRoot::GetSingletonPtr()->GetScriptVM()->ExecuteScriptFunc(kModules,"on_final",true,"ii",this->GetID(),m_nLevel);
				}
			}
		}
	}

	KBOOL ObjectSpawner::CheckAllDead()
	{
		for( KVEC<KUINT>::iterator it = m_kChildList.begin(); 
			 it != m_kChildList.end(); it++)
		{
			Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(*it);
			if (!pEnt)
			{
				it = m_kChildList.erase(it);
			}
		}

		return (m_kChildList.size() == 0);
	}
}