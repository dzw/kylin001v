#pragma once


namespace SkyX
{
	class SkyX;
}

namespace Kylin
{
	class SkyXWapper
	{
	public:
		SkyXWapper(Ogre::SceneManager* pSceneMnger, Ogre::Camera* pCamera);
		~SkyXWapper();

		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID AddGroundPass(Ogre::MaterialPtr spMat);

	protected:
		SkyX::SkyX* m_pSkyX;
	};
}