#include "corepch.h"
#include "DamageSystem.h"
#include "Entity.h"
#include "KylinRoot.h"
#include "RemoteEvents.h"


DamageResult Kylin::DamageSystem::Calculate( const DamageUnit& kDamage, KUINT uEntityID )
{
	DamageResult kResult;
	Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(uEntityID);
	if (pEnt)
	{
		PropertySet& kProp = pEnt->GetPropertyRef();

		KINT nHp;
		if ( kProp.GetIntValue("$HP",nHp) )
		{
			KINT nDamage = Ogre::Math::RangeRandom(kDamage.mMinValue,kDamage.mMaxValue);

			nHp -= nDamage * kDamage.mLevel;
			{
				if (nHp < 0) nHp = 0;
				kProp.SetValue("$HP",nHp);
			}

			kResult.mDamage = nDamage;
			kResult.mDIFF   = nHp;
		}
	}

	return kResult;
}