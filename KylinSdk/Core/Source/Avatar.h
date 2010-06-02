#pragma once

namespace Kylin
{
	class Avatar
	{
	public:
		
		enum AvatarPart
		{
			AP_CHEST,
			AP_HELMET,
			AP_FACE,
			AP_SHOULDERS,
			AP_GLOVES,
			AP_BOOTS,
		};

		virtual KVOID Exchange(Node* pNode, KUINT uGID);
		virtual KVOID Exchange(Ogre::Entity* pHost, AvatarPart eType, KSTR sMat);

	};
}