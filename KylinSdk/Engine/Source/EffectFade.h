#pragma once

#include "EffectManager.h"


namespace Kylin
{
	class EffectFade : public EffectCompositor
					 , public Ogre::CompositorInstance::Listener
	{
	public:
		EffectFade():EffectCompositor("Fade"){}
		virtual KBOOL Initialize();
		virtual KVOID Destroy();
		virtual KVOID Render(KFLOAT fElapsed);
		virtual KVOID Activate(KBOOL bFlag);
		virtual KBOOL IsVisible();

	protected:
		void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
		{
			if (pass_id == 0xDEADBADE)
			{
				m_spFProgParam = mat->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
			}
		}

		void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
		{
			if (pass_id == 0xDEADBADE)
			{
				// update gpu program parameters
				m_spFProgParam->setNamedConstant("colour_amount", m_fColourAmount);
				m_spFProgParam->setNamedConstant("gray_amount", m_fGrayAmount);
			}
		}

	protected:
		KFLOAT		m_fColourAmount;
		KFLOAT		m_fGrayAmount;
		
		KBOOL		m_bActived;
		KINT		m_nDelay;

		Ogre::GpuProgramParametersSharedPtr m_spFProgParam;
	};
}