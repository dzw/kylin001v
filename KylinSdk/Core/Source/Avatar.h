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
		
		// 挂载武器
		virtual Node* AttachWeapon(KUINT uGID, AvatarPart ePart = AP_RWEAPON);
		// 卸载武器
		virtual KVOID DetachWeapon(AvatarPart ePart);

		Node*	GetRWeaponNode();
		
		// 换装
		virtual KBOOL Exchange(KUINT uGID);
		virtual KBOOL Exchange(Ogre::Entity* pHost, AvatarPart eType, KSTR sMat);
	
		// 绑定刀光
		KVOID		BindWeaponTrail(AvatarPart ePart = AP_RWEAPON, KINT nWidth = 2);
		KVOID		SetWeaponTrailVisible(KBOOL bFlag);
	
		virtual KVOID Update(KFLOAT fElapsed);

		// 刷新属性
		virtual KVOID RefreshProp(KUINT uID);

	protected:
		Node*	m_pLWeapon;
		Node*	m_pRWeapon;

		//WeaponTrail*	m_pLWeaponTrail;		// 只有右手武器， 无左手武器
		WeaponTrail*	m_pRWeaponTrail;

		//KUINT	m_uLWeapon;
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