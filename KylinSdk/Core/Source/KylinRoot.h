#pragma once

#include "Singleton.h"
#include "Property.h"
#include "Event.h"


namespace Kylin
{
	class KylinRoot : public Singleton<KylinRoot>
	{
	public:
		//����ID���entity
		Entity *	GetEntity(KUINT uID);
		
		//��ÿ��ָ��
		GameFrame*	GetGameFramePtr();
		
		//�������������Ƿ��ཻ
		KBOOL		HitTest(const KPoint3& vOrg, const KPoint3& vDir, KPoint3& vPicked);
		KBOOL		HitTest(const Ogre::Ray &kCamRay, KPoint3& vRetPos);

		//�����Բ���entity
		Entity *	SpawnEntity(PropertySet& rProp);
		
		//��entity������Ϣ
		KVOID		PostMessage(KUINT uEntID,const EventPtr spEvent);

	protected:

	};
}
