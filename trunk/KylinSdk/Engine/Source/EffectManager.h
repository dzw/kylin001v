#pragma once

#include "Singleton.h"


namespace Kylin
{
	class EffectObject
	{
	public:
		virtual KVOID Initialize();
		virtual KVOID Destroy();
		virtual KVOID Update(KFLOAT fElapsed);

		virtual KVOID Attach(Node* pNode);
		virtual KVOID DetachParent();
		
		Node*	m_pParent;
		KSTR	m_sName;
	};

	class EffectParticle : public EffectObject
	{
	public:
		virtual KVOID Initialize()
		{
			//pTemplate = mSceneManager->createParticleSystem("Spark", "Spark");
			//mSceneManager->getRootSceneNode()->attachObject(pTemplate);
		}
		
		virtual KVOID Destroy();
		
		virtual KVOID Attach(Node* pNode);
		
		virtual KVOID DetachParent();

	private:
		Ogre::ParticleSystem* pTemplate;
	};

	class EffectManager : public Singleton<EffectManager>
	{
	public:
		KBOOL Initialize(Ogre::Root* pRoot, Ogre::SceneManager* pSceneMngr);
		KVOID Destroy();
		KVOID Update(KFLOAT fElapsed);

		// temp 
		// KVOID CreateLightning();
		
		// 
		KVOID SetEffectEnable(KSTR sName, KBOOL bFlag);

	protected:
		KVOID CreateCompositors();

	protected:

	};
}