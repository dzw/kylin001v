#pragma once


namespace Kylin
{
	class OgreUtils
	{
	public:
		static KPoint3	GetEntitySize(Ogre::MovableObject* pObj, KFLOAT fScale = 1.0f);

		// ��̬����ģ��
		static KVOID	DynamicLoadMesh(KSTR sMesh);
	};
}