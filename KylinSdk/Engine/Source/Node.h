#pragma once

#include "Engine.h"
#include "property.h"

#include "CollisionWrapper.h"

namespace Kylin
{
	class Node : public CollisioListener
	{
	public:
		Node();
		virtual ~Node();

		virtual KBOOL Load(PropertySet kProp);
		virtual KVOID Unload();
		virtual KVOID Destroy();

		virtual KVOID Tick(KFLOAT fElapsed);
		
// 		Node*	CreateChildNode(KCCHAR* pNodeName);
// 		KVOID	DestroyChildNode(KCCHAR* pNodeName);
// 		Node*	QueryChildNode(KCCHAR* pNodeName);
// 
// 		KVOID	AttachNode(Node* pNode,KCCHAR* pNodeName = NULL);
// 		KVOID	DetachNode(Node* pNode);
// 		KVOID	DetachParentNode();
// 
		KVOID	AttachEffect(KSTR sName, KUINT uType = 1, KPoint3 kPos = KPoint3::ZERO);
		KVOID	DetachAndDestroyEffect(KSTR sName);
		KVOID	ActivateEffect(KSTR sName, KBOOL bFlag);

// 		KUINT	GetChildrenCount();
// 		Node*	GetChildByIndex(KUINT uIndex);
// 		Node*	GetParent();
// 
 		virtual KVOID SetTranslate(KPoint3 kPos);
 		KPoint3 GetTranslate();

		virtual KVOID SetRotation(KQuaternion kQua);
		KQuaternion GetRotation();
// 
 		KVOID	 SetYaw(KFLOAT fYaw);
 		KMatrix3 GetRotate();
// 
// 		virtual KVOID SetTransparency(KFLOAT fAlph);
// 		KFLOAT	GetTransparency();
// 
 		virtual KVOID SetVisible(KBOOL bFlag);
 		KBOOL	GetVisible();
// 		
 		virtual KVOID	SetScale(KFLOAT fScale);
 		KFLOAT	GetScale();
// 
// 		KPoint3 GetBoundCenter();
// 		KPoint3 GetBoundRadius();
// 		
 		Ogre::SceneNode* GetSceneNode();
		Ogre::Entity*	 GetEntityPtr();

		KSTR	GetEntityName() { return m_pOgreEntity->getName(); }
		
		KVOID	SetWorldID(KUINT uID){ m_uWID = uID; }
		KUINT	GetWorldID() { return m_uWID; }

		AnimationProxy*	 GetAnimationProxy();

	protected:
		KUINT			 m_uWID;
		Ogre::SceneNode* m_pOgreNode;
		Ogre::Entity*	 m_pOgreEntity;
		AnimationProxy*	 m_pAnimProxy;

		//---------------------------------------
		KVEC<EffectObject*> m_kEffectList;
	};
}


