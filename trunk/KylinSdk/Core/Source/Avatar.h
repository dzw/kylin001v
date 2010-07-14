#pragma once

namespace Kylin
{
	class Avatar
	{
	public:
		Avatar(Character* pChar);
		~Avatar();

		enum AvatarPart
		{
			AP_RWEAPON,
			AP_LWEAPON,
			AP_CHEST,
			AP_HELMET,
			AP_FACE,
			AP_SHOULDERS,
			AP_GLOVES,
			AP_BOOTS,
		};
		
		virtual Node* AttachWeapon(KUINT uGID, AvatarPart ePart = AP_RWEAPON);
		virtual KVOID DetachWeapon(AvatarPart ePart);

		Node*	GetRWeaponNode();

		virtual KVOID Exchange(KUINT uGID);
		virtual KVOID Exchange(Ogre::Entity* pHost, AvatarPart eType, KSTR sMat);
	
		// 
		KVOID		BindWeaponTrail(AvatarPart ePart = AP_RWEAPON);
		KVOID		SetWeaponTrailVisible(KBOOL bFlag);
	
		virtual KVOID Update(KFLOAT fElapsed);

	protected:
		Node*	m_pLWeapon;
		Node*	m_pRWeapon;

		WeaponTrail*	m_pLWeaponTrail;
		WeaponTrail*	m_pRWeaponTrail;

		KUINT	m_uLWeapon;
		KUINT	m_uRWeapon;
		KUINT	m_uChest;
		KUINT	m_uHelmet;
		KUINT	m_uFace;
		KUINT	m_uShoulders;
		KUINT	m_uGloves;
		KUINT	m_uBoots;
		
		Character* m_pHost;
	};
}