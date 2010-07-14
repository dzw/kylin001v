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
		KylinRoot();

		// 关系
		enum ENUM_RELATION
		{
			RELATION_INVALID	= -1,
			RELATION_ENEMY,				// 敌对
			RELATION_FRIEND,			// 友好

			RELATION_NUMBERS
		};

		enum ENTITY_QUERY_MASK 
		{ 
			KR_NO_MASK = 0,
			KR_SCENE_OBJ = 1<<4,
			KR_NPC_MASK = 1<<5, 
			KR_ITEM_MASK = 1<<6, 
			KR_PLAYER_MASK = 1<<7, 
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
		Entity*		SpawnCharactor(KUINT uGid, KUINT uCid);
		// 创建道具
		Entity*		SpawnItem(KUINT uGid,KUINT uCid);

		// 创建装备等
		
		// 向entity发送消息
		KVOID		PostMessage(KUINT uEntID,const EventPtr spEvent);
		// 切换状态
		KVOID		SwitchStatus(GameStatus* pStatus);
		// 切换场景
		KVOID		SwitchScene(KUINT uSceneID);

		// debug
		KVOID		DebugHideEntities(KBOOL bFlag);
		// debug
		KVOID		DebugHideTerrain(KBOOL bFlag);
		// debug	
		KVOID		DebugShowBoundingBox(KBOOL bFlag);
		// 获得当前状态
		GameStatus*	GetCurrentGameStatus();
		// 获得当前场景
		Scene*		GetCurrentScene();
		//
		KVOID		SetActiveCamera(GameCamera* pCam);
		GameCamera* GetActiveCamera();
		
		// 通知脚步中的entity 函数
		KVOID		NotifyScriptEntity(Kylin::Entity* pEnt, KCSTR& sFunc);
		
		// 设置鼠标指针类型
		KVOID		SetMousePointer(KUINT uType);
		
		// 获得游戏时间
		KFLOAT		GetGameTime();
	
		// 检测对象间关系是否可以攻击
		KUINT		CheckRelation(Kylin::Character* pEnt1,Kylin::Character* pEnt2);

	protected:
		GameCamera* m_pCamera;
	};
}
