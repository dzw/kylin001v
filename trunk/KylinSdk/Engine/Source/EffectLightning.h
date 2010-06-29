#pragma once

#include "EffectManager.h"

namespace Kylin
{
	class EffectLightning : public EffectObject
	{
	public:
		EffectLightning(KSTR sName);

		// 初始化
		virtual KBOOL Initialize();

		virtual KVOID Attach(const KPoint3& kStart,const KPoint3& kEnd);

		virtual KVOID Destroy();

		// 激活特效
		virtual KVOID Activate(KBOOL bFlag);

		virtual KVOID Render(KFLOAT fElapsed);
		
		// 是否可见
		virtual KBOOL IsVisible();

	private:
		Ogre::BillboardChain *m_pBillboardChain;

		Ogre::Vector3 mStartPos;
		Ogre::Vector3 mEndPos;
		Ogre::Vector3 mDistance;
	
		Ogre::SceneNode* m_pSceneNode;
	};
}