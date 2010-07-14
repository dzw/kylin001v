#include "corepch.h"

#include ".\scriptfunctions.h"
#include "ScriptVM.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "Entity.h"
#include "AnimationProxy.h"
#include "Character.h"
#include "Avatar.h"
#include "NpcObject.h"
#include "rOgreUtils.h"
#include "ObjectSpawner.h"
#include "RemoteEvents.h"
#include "Level.h"


namespace Script
{

	void lua_break(const char * sDebugInfo)
	{
		OutputDebugString(sDebugInfo);
		DebugBreak();
	}
	

	extern void set_next_anim( unsigned int uEntID, const char* sAnim, float fTimes )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		SAFE_CALL(pEnt->GetAnimationProxy(),Play(sAnim,fTimes < 0));
	}

	extern void add_effect( unsigned int uEntID, unsigned int uEffectID)
	{
		int i = 0;
	}

	extern void add_global_effect( unsigned int uEffectID )
	{
		int i = 0;
	}

	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Character,pEnt))
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);
			pChar->GetAvatar()->Exchange(uAvatarID);
		}
	}

	extern void add_pathway_pos( unsigned int uEntID, float x, float z )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::NpcObject,pEnt))
		{
			Kylin::NpcObject* pNpc = BtStaticCast(Kylin::NpcObject,pEnt);
			SAFE_CALL(pNpc->GetAIHandler(),AddPathwayPos(KPoint3(x,0,z)));	
		}
	}

	extern void query_near_foeman( unsigned int uEntID, float r )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::NpcObject,pEnt))
		{
			Kylin::NpcObject* pNpc = BtStaticCast(Kylin::NpcObject,pEnt);
			
			KVEC<Ogre::Entity*> kEntities;
			Kylin::OgreUtils::SphereQuery(pNpc->GetTranslate(),r,kEntities,Kylin::KylinRoot::KR_NPC_MASK);

			float fDistance = 99999999.0f;
			for (KUINT i = 0; i < kEntities.size(); i++)
			{
				
			}
		}
	}

	extern void set_spawner( unsigned int uEntID, float fInterval, float fDelay, int nMaxCount, unsigned int uSpawnID, bool bFlag)
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::ObjectSpawner,pEnt))
		{
			Kylin::ObjectSpawner* pObj = BtStaticCast(Kylin::ObjectSpawner,pEnt);

			pObj->Accept(fInterval,fDelay,nMaxCount,uSpawnID,bFlag);
		}
	}

	extern void set_translate( unsigned int uEntID, float x, float z )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);
		
		KPoint3 kPos;
		if ( Kylin::KylinRoot::GetSingletonPtr()->HitTest(KPoint3(x,0,z),KPoint3::NEGATIVE_UNIT_Y,kPos) )
			SAFE_CALL(pEnt,SetTranslate(kPos));
	}

	extern void add_anim_queue( unsigned int uEntID, const char* sAnim )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		SAFE_CALL(pEnt->GetAnimationProxy(),AddQueue(sAnim));
	}

	extern void kill_character( unsigned int uEntID, unsigned int uKiller )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);
		
		if (pEnt)
		{
			EventPtr spEV(
				KNEW Event(
				&ev_post_killed, 
				Event::ev_immediate, 
				0, 
				1, 
				EventArg(uKiller)
				));

			Kylin::KylinRoot::GetSingletonPtr()->PostMessage(uEntID,spEV);
		}
	}

	extern void set_timer( unsigned int uEntID, float fTimeStep )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Level,pEnt))
		{
			Kylin::Level* pLevel = BtStaticCast(Kylin::Level,pEnt);

			pLevel->SetTimer(fTimeStep);
		}
	}

	extern void kill_timer( unsigned int uEntID )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Level,pEnt))
		{
			Kylin::Level* pLevel = BtStaticCast(Kylin::Level,pEnt);

			pLevel->KillTimer();
		}
	}

	extern void destroy_entity( unsigned int uEntID , float fTimeDelay )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (pEnt)
		{
			// 发送销毁消息
			EventPtr spEV(
				KNEW Event(
				&ev_post_destroy, 
				Event::ev_timing, 
				fTimeDelay, 
				0, 
				NULL
				));

			Kylin::KylinRoot::GetSingletonPtr()->PostMessage(uEntID,spEV);
		}
	}
}