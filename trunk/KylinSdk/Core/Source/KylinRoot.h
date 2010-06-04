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

		// ����ID���entity
		Entity *	GetEntity(KUINT uID);
		
		// ��ÿ��ָ��
		GameFrame*	GetGameFramePtr();
		
		// �������������Ƿ��ཻ
		KBOOL		HitTest(const KPoint3& vOrg, const KPoint3& vDir, KPoint3& vPicked);
		KBOOL		HitTest(const Ogre::Ray &kCamRay, KPoint3& vRetPos);

		// �����Բ���entity
		Entity *	SpawnEntity(PropertySet& rProp);
		KVOID		DestroyEntity(KUINT uEntID);
		// ������ɫ
		Entity*		SpawnCharactor(KUINT uGid, ClassID uCid);
		// ����װ����
		

		// ��entity������Ϣ
		KVOID		PostMessage(KUINT uEntID,const EventPtr spEvent);
		// �л�״̬
		KVOID		SwitchStatus(GameStatus* pStatus);

		// debug
		KVOID		DebugHideEntities(KBOOL bFlag);
		// debug
		KVOID		DebugHideTerrain(KBOOL bFlag);
		// ��õ�ǰ״̬
		GameStatus*	GetCurrentGameStatus();
		//

	protected:
	};
}
