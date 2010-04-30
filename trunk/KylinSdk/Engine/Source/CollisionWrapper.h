#pragma once

#include "ogreopcode.h"
#include "Singleton.h"
#include "Property.h"

using namespace OgreOpcode;

namespace Kylin
{
	// COLLTYPE_QUICK ��ʾ����charactor֮�����ײʹ�ü򵥵�������м�� 
	// COLLTYPE_IGNORE ��ʾ���Գ�������֮�����ײ
	// COLLTYPE_EXACT ��ʾ����charactor��scene��ײʱ�����н���

	enum CllsnType
	{
		CLT_ENTITY,
		CLT_SCENE,
		CLT_CAMERA,
	};

	enum CllsnShape
	{
		CLS_MESH,
		CLS_ENTITY,
		CLS_BOX,
		CLS_CAPSULE_MESH,
		CLS_SPHERE_MESH,
		CLS_TERRAIN,
	};

	class CollisioListener
	{
	public:
		~CollisioListener();

		virtual KVOID OnEntityCllsn(Node* pCollidee,const KPoint3& rNormal) {}
		virtual KVOID OnSceneCllsn(const KPoint3& rNormal) {}
		virtual KBOOL OnShouldCllsn(Node* pCollidee) { return false; }
		//virtual KVOID OnSeparate(Node* pCollidee = NULL) {}
		
	public:
		KSTR	GetName() { return m_sName; }

	protected:
		virtual KBOOL SetupCllsn(Ogre::Entity* pEnt,PropertySet kProp );
		
	protected:
		OgreOpcode::ICollisionShape* m_pCllsnShape;
		OgreOpcode::CollisionObject* m_pCllsnObject;

		KSTR	m_sName;
	};


	class CollisionWrapper : public Singleton<CollisionWrapper>
	{
	public:
		CollisionWrapper();
		virtual ~CollisionWrapper();
		
	public:
		virtual KBOOL Initialize(SceneManager* pSceneMgr);
		virtual KVOID Update(KFLOAT fElapsed);
		virtual KVOID Destroy();

	public:
		KVOID	ShowCllsnFrame(KBOOL bFlag);
		
		KVOID	RegisterCollider(CollisioListener* pCollider);
		KVOID	UnregisterCollider(CollisioListener* pCollider);

	//protected:
		KINT	Query(const Ogre::Ray kCollideRay,KFLOAT fCllsnDist);
		CollisioListener* FindCollider(KSTR sName);

	protected:
		KBOOL	m_bDispCllsnFrame;

		typedef std::list<CollisioListener*> ColliderList;
		ColliderList m_kColliders;
	};
}