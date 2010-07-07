#include "corepch.h"
#include "DamageSystem.h"
#include "Entity.h"
#include "KylinRoot.h"
#include "RemoteEvents.h"


KINT Kylin::DamageSystem::Calculate( const DamageUnit& kDamage, KUINT uEntityID )
{
	Kylin::Entity* pEnt = KylinRoot::GetSingletonPtr()->GetEntity(uEntityID);
	if (pEnt)
	{
		PropertySet& kProp = pEnt->GetPropertyRef();

		KINT nHp;
		if ( kProp.GetIntValue("$HP",nHp) )
		{
			KINT nDamage = Ogre::Math::RangeRandom(kDamage.mMinValue,kDamage.mMaxValue);

			nHp -= nDamage * kDamage.mLevel;

			if (nHp <= 0)
			{
				// ·¢ËÍËÀÍöÏûÏ¢
				EventPtr spEV(
					new Event(
					&ev_post_killed, 
					Event::ev_immediate, 
					0, 
					0, 
					NULL
					));

				KylinRoot::GetSingletonPtr()->PostMessage(uEntityID,spEV);
			}
			else
			{
				kProp.SetValue("$HP",nHp);
			}

			return nDamage;
		}
	}

	return -1;
}