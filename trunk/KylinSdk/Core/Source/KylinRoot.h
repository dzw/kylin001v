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

		enum ENTITY_QUERY_MASK 
		{ 
			KR_NO_MASK = 0,
			KR_SCENE_OBJ = 1<<4,
			KR_NPC_MASK = 1<<5, 
			KR_ITEM_MASK = 1<<6, 
			KR_PLAYER_MASK = 1<<7, 
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
		// �л�����
		KVOID		SwitchScene(KUINT uSceneID);

		// debug
		KVOID		DebugHideEntities(KBOOL bFlag);
		// debug
		KVOID		DebugHideTerrain(KBOOL bFlag);
		// debug	
		KVOID		DebugShowBoundingBox(KBOOL bFlag);
		// ��õ�ǰ״̬
		GameStatus*	GetCurrentGameStatus();
		// ��õ�ǰ����
		Scene*		GetCurrentScene();
		//
		KVOID		SetActiveCamera(GameCamera* pCam);
		GameCamera* GetActiveCamera();
		
		// ֪ͨ�Ų��е�entity ����
		KVOID		NotifyScriptEntity(Kylin::Entity* pEnt, KCSTR& sFunc);
		
		// �������ָ������
		KVOID		SetMousePointer(KUINT uType);

	protected:
		GameCamera* m_pCamera;
	};
}