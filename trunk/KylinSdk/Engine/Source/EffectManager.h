#pragma once

#include "Singleton.h"
//#include "ParticleUniverseSystem.h"
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
		// render
		virtual KVOID Render(KFLOAT fElapsed){}
		// 销毁
		virtual KVOID Destroy() = 0;

		// 挂载到某点
		virtual KVOID Attach(Ogre::SceneNode* pNode, KPoint3 kOffset = KPoint3::ZERO, KFLOAT fScale = 1.0f){}
	
		// 激活特效
		virtual KVOID Activate(KBOOL bFlag) = 0;
		virtual KBOOL IsEnabled() { return false; }

		// 获得特效名称
		virtual KSTR  GetName() { return m_sName; }
		// 
		virtual KVOID SetScale(KFLOAT fScale){}

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
		// render
		virtual KVOID Render(KFLOAT fElapsed){}

		virtual KVOID Destroy();

		virtual KVOID Activate(KBOOL bFlag);
		
		virtual KVOID Attach(Ogre::SceneNode* pNode, KPoint3 kOffset = KPoint3::ZERO, KFLOAT fScale = 1.0f);
		
		virtual KVOID SetScale(KFLOAT fScale);

	protected:
		//ParticleUniverse::ParticleSystem*	m_pParticleSystemEx;	// 扩展粒子特效句柄
		Ogre::ParticleSystem*		m_pParticleHandle;		// 
		Ogre::SceneNode*			m_pRoot;

		KSTR	m_sTemplate;								// 特效模板
		KFLOAT	m_fLifeTime;								// 存在时间
		KBOOL	m_bLoopFlag;								// 是否循环播放
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
		
		virtual KBOOL IsEnabled();

	protected:
		Ogre::CompositorInstance* m_pCompositor;
	};

	//////////////////////////////////////////////////////////////////////////
	// Bloom特效
//	class EffectBloom : public EffectCompositor
//	{
//	public:
//		EffectBloom():EffectCompositor("Bloom"){}
//		virtual KBOOL Initialize();
//	};

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
		// render
		KVOID Render(KFLOAT fElapsed);
		// 销毁
		KVOID Destroy();
		// 销毁某个特效
		KVOID DestroyEffect(KSTR sName);
		// 产生特效
		KVOID Generate(EffectObject* pEffect);
		EffectObject* Generate(const KSTR& sName, const KSTR& sTemplate, KUINT uType = ET_PARTICLE);
		// 激活特效
		KVOID Activate(KSTR sName, KBOOL bFlag);

	protected:
		typedef KMAP<KSTR,EffectObject*> EffectMap;
		
		EffectMap m_kEffectMap;	// 特效列表
	};
}