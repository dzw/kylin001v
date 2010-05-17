#pragma once

#include "Singleton.h"
#include "Property.h"
#include "Event.h"


namespace Kylin
{
	class KylinRoot : public Singleton<KylinRoot>
	{
	public:
		//根据ID获得entity
		Entity *	GetEntity(KUINT uID);
		
		//获得框架指针
		GameFrame*	GetGameFramePtr();
		
		//检测射线与地面是否相交
		KBOOL		HitTest(const KPoint3& vOrg, const KPoint3& vDir, KPoint3& vPicked);
		KBOOL		HitTest(const Ogre::Ray &kCamRay, KPoint3& vRetPos);

		//由属性产生entity
		Entity *	SpawnEntity(PropertySet& rProp);
		KVOID		DestroyEntity(KUINT uEntID);

		//向entity发送消息
		KVOID		PostMessage(KUINT uEntID,const EventPtr spEvent);
		
		KVOID		SwitchStatus(GameStatus* pStatus);

		// debug
		KVOID		DebugHideEntities(KBOOL bFlag);
		// 
		KVOID		DebugHideTerrain(KBOOL bFlag);
		// 获得当前状态
		GameStatus*	GetCurrentGameStatus();

	protected:

	};
}
