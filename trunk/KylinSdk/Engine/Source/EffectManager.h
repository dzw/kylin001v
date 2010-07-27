#pragma once

#include "Singleton.h"
//#include "ParticleUniverseSystem.h"
#include "Property.h"


namespace Kylin
{
	// ��Ч����
	enum EffectType
	{
		ET_NONE,
		ET_PARTICLE,
		ET_COMPOSITOR,
		ET_CUSTROM,
	};

	//////////////////////////////////////////////////////////////////////////
	// ��Ч�ӿ�
	class EffectObject
	{
	public:
		EffectObject(KSTR sName)
			: m_sName(sName)
			, m_pClocking(NULL)
			, m_uType(ET_NONE)
			, m_bAutoRemove(false)
		{}
		virtual ~EffectObject(){}

		// ��ʼ��
		virtual KBOOL Initialize() = 0;
		// render
		virtual KVOID Render(KFLOAT fElapsed){}
		// ����
		virtual KVOID Destroy() = 0;

		// ���ص�ĳ��
		virtual KVOID Attach(Ogre::SceneNode* pNode, KFLOAT fScale = 1.0f){}
	
		// ������Ч
		virtual KVOID Activate(KBOOL bFlag) = 0;

		// �����Ч����
		virtual KSTR  GetName() { return m_sName; }
		// 
		virtual KVOID SetScale(KFLOAT fScale){}
		// �Ƿ�ɼ�
		virtual KBOOL IsVisible(){ return false; }
		// ���ûص�����
		virtual KVOID SetCallbackObj(ClockingCallback* pObj);
		// �����û�����
		virtual KVOID SetUserData(KANY aData) { m_kUserData = aData; } 
		// 
		virtual KVOID SetAutoRemove(KBOOL bFlag) { m_bAutoRemove = bFlag; } 
		//
		virtual KVOID SetPosition(const KPoint3& pt){}

	protected:
		KSTR				m_sName;		// ��Ч����
		KUINT				m_uType;		// ��Ч����
		
		KBOOL				m_bAutoRemove;	// �Զ�ɾ��
		ClockingCallback*	m_pClocking;	// ��ʱ�ص�
		KANY				m_kUserData;
	};
	
	//////////////////////////////////////////////////////////////////////////
	// ������Ч
	class EffectParticle : public EffectObject
	{
	public:
		EffectParticle(KSTR sName, KSTR sTemplate, KFLOAT fLifeTime);
		virtual ~EffectParticle();

		virtual KBOOL Initialize();
		// render
		virtual KVOID Render(KFLOAT fElapsed);

		virtual KVOID Destroy();

		virtual KVOID Activate(KBOOL bFlag);
		
		virtual KVOID Attach(Ogre::SceneNode* pNode, KFLOAT fScale = 1.0f);
		
		virtual KVOID SetScale(KFLOAT fScale);
		// �Ƿ�ɼ�
		virtual KBOOL IsVisible();
		
		virtual KVOID SetPosition(const KPoint3& pt);

	protected:
		//ParticleUniverse::ParticleSystem*	m_pParticleSystemEx;	// ��չ������Ч���
		Ogre::ParticleSystem*		m_pParticleHandle;		// 
		Ogre::SceneNode*			m_pRoot;

		KSTR	m_sTemplate;								// ��Чģ��
		KFLOAT	m_fLifeTime;								// ����ʱ�䣬 <= 0 Ϊ����ʱ
		
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

		// ��ʼ��
		KBOOL Initialize();
		// render
		KVOID Render(KFLOAT fElapsed);
		// ����
		KVOID Destroy();
		// ����ĳ����Ч
		KVOID DestroyEffect(KSTR sName);
		// ������Ч
		EffectObject* Generate(EffectObject* pEffect);
		EffectObject* Generate(const KSTR& sName, const KSTR& sTemplate, KFLOAT fLifeTime = -1.0f, KUINT uType = ET_PARTICLE);
		// ������Ч
		KVOID Activate(KSTR sName, KBOOL bFlag);

	protected:
		typedef KMAP<KSTR,EffectObject*> EffectMap;
		
		EffectMap m_kEffectMap;	// ��Ч�б�
	};
}