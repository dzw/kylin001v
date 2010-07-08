#pragma once

#include "GuiManager.h"

namespace Kylin
{
	class LoadingProgress : public GuiBase
						  , public Ogre::ResourceGroupListener
	{
	public:
		LoadingProgress();
		virtual ~LoadingProgress();
		
		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed){}
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);

	protected:
		void resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount);
		void scriptParseStarted(const Ogre::String& scriptName, bool& skipThisScript);
		void scriptParseEnded(const Ogre::String& scriptName, bool skipped);
		void resourceGroupScriptingEnded(const Ogre::String& groupName) {}
		void resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount);
		void resourceLoadStarted(const Ogre::ResourcePtr& resource);
		void resourceLoadEnded();
		void worldGeometryStageStarted(const Ogre::String& description);
		void worldGeometryStageEnded();
		void resourceGroupLoadEnded(const Ogre::String& groupName) {}

	protected:
		Ogre::Overlay*	m_pBackground;

		KFLOAT			m_fInitProportion;
		KWORD			m_dNumGroupsInit;
		KWORD			m_dNumGroupsLoad;
		KFLOAT			m_fProgressBarMaxSize;
		KFLOAT			m_fProgressBarScriptSize;
		KFLOAT			m_fProgressBarInc;
		Ogre::OverlayElement* m_pLoadingBarElement;
	};
}