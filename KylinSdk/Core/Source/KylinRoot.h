#pragma once

#include "Singleton.h"
#include "Property.h"
#include "Event.h"
#include "RegisterClass.h"


namespace Kylin
{
	class KylinRoot : public Singleton<KylinRoot>
	{
	public:
		enum ENTITY_TYPE_MASK 
		{ 
			KR_NO_MASK = 1,
			KR_MONSTER_MASK = 1<<1, 
			KR_ITEM_MASK = 1<<2, 
			KR_NPC_MASK = 1<<3, 
			KR_PLAYER_MASK = 1<<4,
			KR_CHAR_MASK = 1<<5,
			KR_MASK_COUNT 
		}; 

		// 根据ID获得entity
		Entity *	GetEntity(KUINT uID);
		
		// 获得框架指针
		GameFrame*	GetGameFramePtr();
		
		// 检测射线与地面是否相交
		KBOOL		HitTest(const KPoint3& vOrg, const KPoint3& vDir, KPoint3& vPicked);
		KBOOL		HitTest(const Ogre::Ray &kCamRay, KPoint3& vRetPos);

		// 由属性产生entity
		Entity *	SpawnEntity(PropertySet& rProp);
		KVOID		DestroyEntity(KUINT uEntID);
		// 创建角色
		Entity*		SpawnCharactor(KUINT uGid, ClassID uCid);
		// 创建装备等
		

		// 向entity发送消息
		KVOID		PostMessage(KUINT uEntID,const EventPtr spEvent);
		// 切换状态
		KVOID		SwitchStatus(GameStatus* pStatus);

		// debug
		KVOID		DebugHideEntities(KBOOL bFlag);
		// debug
		KVOID		DebugHideTerrain(KBOOL bFlag);
		// 获得当前状态
		GameStatus*	GetCurrentGameStatus();
		//

	protected:
	};
}
