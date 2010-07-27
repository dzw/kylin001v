#pragma once

#include "Singleton.h"
//#include "ParticleUniverseSystem.h"
#include "Property.h"


namespace Kylin
{
	// 特效类型
	enum EffectType
	{
		ET_NONE,
		ET_PARTICLE,
		ET_COMPOSITOR,
		ET_CUSTROM,
	};

	//////////////////////////////////////////////////////////////////////////
	// 特效接口
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

		// 初始化
		virtual KBOOL Initialize() = 0;
		// render
		virtual KVOID Render(KFLOAT fElapsed){}
		// 销毁
		virtual KVOID Destroy() = 0;

		// 挂载到某点
		virtual KVOID Attach(Ogre::SceneNode* pNode, KFLOAT fScale = 1.0f){}
	
		// 激活特效
		virtual KVOID Activate(KBOOL bFlag) = 0;

		// 获得特效名称
		virtual KSTR  GetName() { return m_sName; }
		// 
		virtual KVOID SetScale(KFLOAT fScale){}
		// 是否可见
		virtual KBOOL IsVisible(){ return false; }
		// 设置回调对象
		virtual KVOID SetCallbackObj(ClockingCallback* pObj);
		// 设置用户数据
		virtual KVOID SetUserData(KANY aData) { m_kUserData = aData; } 
		// 
		virtual KVOID SetAutoRemove(KBOOL bFlag) { m_bAutoRemove = bFlag; } 
		//
		virtual KVOID SetPosition(const KPoint3& pt){}

	protected:
		KSTR				m_sName;		// 特效名称
		KUINT				m_uType;		// 特效类型
		
		KBOOL				m_bAutoRemove;	// 自动删除
		ClockingCallback*	m_pClocking;	// 定时回调
		KANY				m_kUserData;
	};
	
	//////////////////////////////////////////////////////////////////////////
	// 粒子特效
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
		// 是否可见
		virtual KBOOL IsVisible();
		
		virtual KVOID SetPosition(const KPoint3& pt);

	protected:
		//ParticleUniverse::ParticleSystem*	m_pParticleSystemEx;	// 扩展粒子特效句柄
		Ogre::ParticleSystem*		m_pParticleHandle;		// 
		Ogre::SceneNode*			m_pRoot;

		KSTR	m_sTemplate;								// 特效模板
		KFLOAT	m_fLifeTime;								// 存在时间， <= 0 为无限时
		
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

		// 初始化
		KBOOL Initialize();
		// render
		KVOID Render(KFLOAT fElapsed);
		// 销毁
		KVOID Destroy();
		// 销毁某个特效
		KVOID DestroyEffect(KSTR sName);
		// 产生特效
		EffectObject* Generate(EffectObject* pEffect);
		EffectObject* Generate(const KSTR& sName, const KSTR& sTemplate, KFLOAT fLifeTime = -1.0f, KUINT uType = ET_PARTICLE);
		// 激活特效
		KVOID Activate(KSTR sName, KBOOL bFlag);

	protected:
		typedef KMAP<KSTR,EffectObject*> EffectMap;
		
		EffectMap m_kEffectMap;	// 特效列表
	};
}