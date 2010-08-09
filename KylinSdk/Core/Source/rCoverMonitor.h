#pragma once


namespace Kylin
{
	namespace PhyX
	{
		//////////////////////////////////////////////////////////////////////////
		// ÕÚµ²´¦Àí
		class CoverMonitor
		{
		public:
			CoverMonitor();
			
			virtual KVOID	Tick(KFLOAT fElapsed);
			virtual KVOID	Destroy();

		protected:
			typedef KLIST<EntityMaterialInstance*> CoverObjs;
			virtual KVOID	Testing(KFLOAT fElapsed);
			
			KBOOL			IsInclude(Ogre::Entity* pEnt);

		protected:
			KFLOAT		m_fTimespan;

			CoverObjs	m_kCoverObj;
		};
	}
}