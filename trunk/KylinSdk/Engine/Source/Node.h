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


		virtual KVOID Tick(KFLOAT fElapsed);
		
// 		Node*	CreateChildNode(KCCHAR* pNodeName);
// 		KVOID	DestroyChildNode(KCCHAR* pNodeName);
// 		Node*	QueryChildNode(KCCHAR* pNodeName);
// 
// 		KVOID	AttachNode(Node* pNode,KCCHAR* pNodeName = NULL);
// 		KVOID	DetachNode(Node* pNode);
// 		KVOID	DetachParentNode();
// 
// 		KUINT	GetChildrenCount();
// 		Node*	GetChildByIndex(KUINT uIndex);
// 		Node*	GetParent();
// 
 		virtual KVOID SetTranslate(KPoint3 kPos);
 		KPoint3 GetTranslate();
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
		KSTR	GetEntityName() { return m_pOgreEntity->getName(); }

		AnimationProxy*	 GetAnimationProxy();

	protected:
		Ogre::SceneNode* m_pOgreNode;
		Ogre::Entity*	 m_pOgreEntity;
		AnimationProxy*	 m_pAnimProxy;
	};
}


