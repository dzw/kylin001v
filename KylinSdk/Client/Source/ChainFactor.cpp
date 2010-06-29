#include "cltpch.h"
#include "ChainFactor.h"
#include "clRegisterClass.h"
#include "DataManager.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"
#include "EffectManager.h"
#include "Action.h"
#include "EffectLightning.h"


namespace Kylin
{
	BtImplementRTTI(ChainFactor, Factor, id_chain_factor);

	Implement_Event_Handler(ChainFactor, Factor)
	{
		{NULL, NULL}
	};

	ChainFactor::ChainFactor()
		: m_uTarget(INVALID_ID)
	{

	}

	KBOOL ChainFactor::Init( const PropertySet& kProp )
	{
		if ( !Factor::Init(kProp) )
			return false;

		return true;
	}

	KVOID ChainFactor::PostSpawn()
	{
		//Factor::PostSpawn();
		// ¼¤»îÌØÐ§

		Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(m_spHostAct->GetHostWorldID());
		Kylin::Entity* pTgt = KylinRoot::GetSingletonPtr()->GetEntity(m_uTarget);
		if (pEnt && pTgt)
		{
			KSTR sName = "factor_lightning_" + Ogre::StringConverter::toString(this->GetID());
			EffectLightning* pEffect = static_cast<EffectLightning*>(EffectManager::GetSingletonPtr()->Generate(KNEW EffectLightning(sName)));

			pEffect->Attach(KPoint3(pTgt->GetTranslate().x,1000.0f,pTgt->GetTranslate().z),pTgt->GetTranslate());
		}

		EventPtr spPosEV(
			new Event(
			&ev_post_destroy, 
			Event::ev_timing,
			1, 
			0, 
			NULL
			));

		KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spPosEV);
	}

	KVOID ChainFactor::PostDestroy()
	{
		KSTR sName = "factor_lightning_" + Ogre::StringConverter::toString(this->GetID());
		EffectManager::GetSingletonPtr()->DestroyEffect(sName);

		Factor::PostDestroy();
	}

	KVOID ChainFactor::SetTarget( KUINT uTarget )
	{
		m_uTarget = uTarget;
	}

	KVOID ChainFactor::Tick( KFLOAT fElapsed )
	{
		Factor::Tick(fElapsed);
	}

}

