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

		if (pEnt)
		{
			pEnt->GetAnimationProxy()->Play(sAnim,fTimes < 0);
		}
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

	extern void set_spawner( unsigned int uEntID, float fInterval, float fDelay, int nMaxCount, unsigned int uSpawnID )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::ObjectSpawner,pEnt))
		{
			Kylin::ObjectSpawner* pObj = BtStaticCast(Kylin::ObjectSpawner,pEnt);

			pObj->Accept(fInterval,fDelay,nMaxCount,uSpawnID);
		}
	}

	extern void set_translate( unsigned int uEntID, float x, float z )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		SAFE_CALL(pEnt,SetTranslate(KPoint3(x,0,z)));
	}
}