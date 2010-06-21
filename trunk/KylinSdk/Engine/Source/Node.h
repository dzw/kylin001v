#pragma once

#include "property.h"

#include "CollisionWrapper.h"

namespace Kylin
{
	class Node : public CollisioListener
	{
	public:
		Node();
		virtual ~Node();

		virtual KBOOL			Load(PropertySet kProp);
		virtual KVOID			Unload();
		virtual KVOID			Destroy();
		virtual KVOID			BindEffect(PropertySet kProp){}

		virtual KVOID			Tick(KFLOAT fElapsed);
		
		virtual KBOOL			AttachMesh(Ogre::Entity* pEnt, KSTR sBone);
		virtual KVOID			DetachMesh(Ogre::Entity* pEnt);

		Kylin::EffectObject*	AttachEffect(KSTR sName, KFLOAT fTime = -1.0f, KUINT uType = 1);
		KVOID					DetachAndDestroyEffect(KSTR sName);
		KVOID					ActivateEffect(KSTR sName, KBOOL bFlag);
		
 		virtual KVOID			SetTranslate(KPoint3 kPos);
 		KPoint3					GetTranslate();

		virtual KVOID			SetRotation(KQuaternion kQua);
		KQuaternion				GetRotation();

		KVOID					SetYaw(KFLOAT fYaw);
 		KMatrix3				GetRotate();

		virtual KVOID			SetTransparency(KFLOAT fAlph);
 		KFLOAT					GetTransparency();

		virtual KVOID			SetVisible(KBOOL bFlag);
 		KBOOL					GetVisible();

		virtual KVOID			SetScale(KFLOAT fScale);
 		KFLOAT					GetScale();

 		Ogre::SceneNode*		GetSceneNode();
		Ogre::Entity*			GetEntityPtr();

		KSTR					GetEntityName() { return m_pOgreEntity->getName(); }
		
		KVOID					SetWorldID(KUINT uID){ m_uWID = uID; }
		KUINT					GetWorldID() { return m_uWID; }

		AnimationProxy*			GetAnimationProxy();

	protected:
		KUINT				m_uWID;
		Ogre::SceneNode*	m_pOgreNode;
		Ogre::Entity*		m_pOgreEntity;
		AnimationProxy*		m_pAnimProxy;
		
		EntityMaterialInstance* m_pTransparency;

		//---------------------------------------
		KVEC<EffectObject*> m_kEffectList;
	};
}


