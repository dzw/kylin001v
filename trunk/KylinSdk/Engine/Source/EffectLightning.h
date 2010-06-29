#pragma once

#include "EffectManager.h"

namespace Kylin
{
	class EffectLightning : public EffectObject
	{
	public:
		EffectLightning(KSTR sName);

		// ��ʼ��
		virtual KBOOL Initialize();

		virtual KVOID Attach(const KPoint3& kStart,const KPoint3& kEnd);

		virtual KVOID Destroy();

		// ������Ч
		virtual KVOID Activate(KBOOL bFlag);

		virtual KVOID Render(KFLOAT fElapsed);
		
		// �Ƿ�ɼ�
		virtual KBOOL IsVisible();

	private:
		Ogre::BillboardChain *m_pBillboardChain;

		Ogre::Vector3 mStartPos;
		Ogre::Vector3 mEndPos;
		Ogre::Vector3 mDistance;
	
		Ogre::SceneNode* m_pSceneNode;
	};
}