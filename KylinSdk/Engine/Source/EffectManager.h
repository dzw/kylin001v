#pragma once

#include "Singleton.h"
#include "ParticleUniverseSystem.h"
#include "Property.h"


namespace Kylin
{
	//////////////////////////////////////////////////////////////////////////
	// ��Ч�ӿ�
	class EffectObject
	{
	public:
		EffectObject(KSTR sName)
			: m_sName(sName)
		{}
		virtual ~EffectObject(){}

		// ��ʼ��
		virtual KBOOL Initialize() = 0;
		// render
		virtual KVOID Render(KFLOAT fElapsed){}
		// ����
		virtual KVOID Destroy() = 0;

		// ���ص�ĳ��
		virtual KVOID Attach(Ogre::SceneNode* pNode, KPoint3 kOffset = KPoint3::ZERO){}
	
		// ������Ч
		virtual KVOID Activate(KBOOL bFlag) = 0;
		virtual KBOOL IsEnabled() { return false; }

		// �����Ч����
		virtual KSTR  GetName() { return m_sName; }
		// 
		virtual KVOID SetScale(KFLOAT fScale){}

	protected:
		KSTR				m_sName;	// ��Ч����
		KUINT				m_uType;	// ��Ч����
	};
	
	//////////////////////////////////////////////////////////////////////////
	// ������Ч
	class EffectParticle : public EffectObject
	{
	public:
		EffectParticle(KSTR sName, KSTR sTemplate);
		virtual ~EffectParticle();

		virtual KBOOL Initialize();

		virtual KVOID Destroy();

		virtual KVOID Activate(KBOOL bFlag);
		
		virtual KVOID Attach(Ogre::SceneNode* pNode, KPoint3 kOffset = KPoint3::ZERO);
		
		virtual KVOID SetScale(KFLOAT fScale);

	protected:
		ParticleUniverse::ParticleSystem* m_pSystem;	// ������Ч���
		KSTR	m_sTemplate;							// ��Чģ��
	};
	
	//////////////////////////////////////////////////////////////////////////
	// �ϳ�����Ч
	class EffectCompositor : public EffectObject
	{
	public:
		EffectCompositor(KSTR sName);
		virtual ~EffectCompositor();

		virtual KBOOL Initialize();

		virtual KVOID Destroy();

		virtual KVOID Activate(KBOOL bFlag);
		
		virtual KBOOL IsEnabled();

	protected:
		Ogre::CompositorInstance* m_pCompositor;
	};

	//////////////////////////////////////////////////////////////////////////
	// Bloom��Ч
//	class EffectBloom : public EffectCompositor
//	{
//	public:
//		EffectBloom():EffectCompositor("Bloom"){}
//		virtual KBOOL Initialize();
//	};

	//////////////////////////////////////////////////////////////////////////
	// ��Ч������
	class EffectManager : public Singleton<EffectManager>
	{
	public:
		// ��Ч����
		enum EffectType
		{
			ET_NONE,
			ET_PARTICLE,
			ET_COMPOSITOR,
		};
		// ��ʼ��
		KBOOL Initialize();
		// render
		KVOID Render(KFLOAT fElapsed);
		// ����
		KVOID Destroy();
		// ����ĳ����Ч
		KVOID DestroyEffect(KSTR sName);
		// ������Ч
		KVOID Generate(EffectObject* pEffect);
		EffectObject* Generate(const KSTR& sName, const KSTR& sTemplate, KUINT uType = ET_PARTICLE);
		// ������Ч
		KVOID Activate(KSTR sName, KBOOL bFlag);

	protected:
		typedef KMAP<KSTR,EffectObject*> EffectMap;
		
		EffectMap m_kEffectMap;	// ��Ч�б�
	};
}