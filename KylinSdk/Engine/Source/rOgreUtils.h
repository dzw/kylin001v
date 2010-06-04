#pragma once


namespace Kylin
{
	class OgreUtils
	{
	public:
		static KPoint3	GetEntitySize(Ogre::MovableObject* pObj, KFLOAT fScale = 1.0f);

		// 动态加载模型
		static KVOID	DynamicLoadMesh(KSTR sMesh);
	};
}