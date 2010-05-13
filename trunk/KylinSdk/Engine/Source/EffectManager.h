#pragma once

#include "Singleton.h"
#include "ParticleUniverseSystem.h"
#include "Property.h"


namespace Kylin
{
	//////////////////////////////////////////////////////////////////////////
	// 特效接口
	class EffectObject
	{
	public:
		EffectObject(KSTR sName)
			: m_sName(sName)
		{}
		virtual ~EffectObject(){}

		// 初始化
		virtual KBOOL Initialize() = 0;

		// 销毁
		virtual KVOID Destroy() = 0;

		// 挂载到某点
		virtual KVOID Attach(Ogre::SceneNode* pNode){}

		// 激活特效
		virtual KVOID Activate(KBOOL bFlag) = 0;

		// 获得特效名称
		virtual KSTR  GetName() { return m_sName; }

	protected:
		KSTR				m_sName;	// 特效名称
		KUINT				m_uType;	// 特效类型
	};
	
	//////////////////////////////////////////////////////////////////////////
	// 粒子特效
	class EffectParticle : public EffectObject
	{
	public:
		EffectParticle(KSTR sName, KSTR sTemplate);
		virtual ~EffectParticle();

		virtual KBOOL Initialize();

		virtual KVOID Destroy();

		virtual KVOID Activate(KBOOL bFlag);
		
		virtual KVOID Attach(Ogre::SceneNode* pNode);

	private:
		ParticleUniverse::ParticleSystem* m_pSystem;	// 粒子特效句柄
		KSTR	m_sTemplate;							// 特效模板
	};
	
	//////////////////////////////////////////////////////////////////////////
	// 合成器特效
	class EffectCompositor : public EffectObject
	{
	public:
		EffectCompositor(KSTR sName);
		virtual ~EffectCompositor();

		virtual KBOOL Initialize();

		virtual KVOID Destroy();

		virtual KVOID Activate(KBOOL bFlag);

	private:
		Ogre::CompositorInstance* m_pCompositor;
	};

	//////////////////////////////////////////////////////////////////////////
	// 动态模糊特效
	class EffectMotionBlur : public EffectCompositor
	{
	public:
		EffectMotionBlur():EffectCompositor(""){}
		virtual KBOOL Initialize();
	};

	//////////////////////////////////////////////////////////////////////////
	// 特效管理器
	class EffectManager : public Singleton<EffectManager>
	{
	public:
		// 特效类型
		enum EffectType
		{
			ET_NONE,
			ET_PARTICLE,
			ET_COMPOSITOR,
		};
		// 初始化
		KBOOL Initialize();
		// 销毁
		KVOID Destroy();
		// 销毁某个特效
		KVOID DestroyEffect(KSTR sName);
		// 产生特效
		KVOID Generate(EffectObject* pEffect);
		EffectObject* Generate(PropertySet& kInfo, KUINT uType = ET_PARTICLE);
		// 激活特效
		KVOID Activate(KSTR sName, KBOOL bFlag);

	protected:
		typedef KMAP<KSTR,EffectObject*> EffectMap;
		
		EffectMap m_kEffectMap;	// 特效列表
	};
}