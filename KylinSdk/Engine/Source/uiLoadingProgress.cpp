#include "engpch.h"
#include "uiLoadingProgress.h"
#include "rOgreRoot.h"


Kylin::LoadingProgress::LoadingProgress()
: GuiBase(CLASS_TO(LoadingProgress))
, m_pBackground(NULL)
, m_dNumGroupsInit(1)
, m_dNumGroupsLoad(1)
, m_fInitProportion(0.8f)
{

}

Kylin::LoadingProgress::~LoadingProgress()
{
	
}

KBOOL Kylin::LoadingProgress::Initialize()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Bootstrap");
	m_pBackground				= Ogre::OverlayManager::getSingleton().getByName("Overlays/Splash");
	m_pLoadingBarElement		= Ogre::OverlayManager::getSingleton().getOverlayElement("SplashPanel/Bar/Progress");
	
	Ogre::OverlayElement* barContainer = Ogre::OverlayManager::getSingleton().getOverlayElement("SplashPanel/Bar");
	m_fProgressBarMaxSize = barContainer->getWidth();
	m_pLoadingBarElement->setWidth(0);
	
	SetVisible(false);
	//////////////////////////////////////////////////////////////////////////
	Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(this);

	return true;
}

KVOID Kylin::LoadingProgress::Destroy()
{
	Ogre::ResourceGroupManager::getSingleton().removeResourceGroupListener(this);

	//////////////////////////////////////////////////////////////////////////
}

KVOID Kylin::LoadingProgress::SetVisible( KBOOL bVisible )
{
	GuiBase::SetVisible(bVisible);

	if (bVisible)
		m_pBackground->show();
	else
		m_pBackground->hide();

	OgreRoot::GetSingletonPtr()->GetMainWindow()->update();
}

//////////////////////////////////////////////////////////////////////////
void Kylin::LoadingProgress::resourceGroupScriptingStarted( const Ogre::String& groupName, size_t scriptCount )
{
	m_fProgressBarInc = m_fProgressBarMaxSize * m_fInitProportion / (KFLOAT)scriptCount;
	m_fProgressBarInc /= m_dNumGroupsInit;
	
	OgreRoot::GetSingletonPtr()->GetMainWindow()->update();
}

void Kylin::LoadingProgress::scriptParseStarted( const Ogre::String& scriptName, bool& skipThisScript )
{
	OgreRoot::GetSingletonPtr()->GetMainWindow()->update();
}

void Kylin::LoadingProgress::scriptParseEnded( const Ogre::String& scriptName, bool skipped )
{
	m_pLoadingBarElement->setWidth(
		m_pLoadingBarElement->getWidth() + m_fProgressBarInc);

	OgreRoot::GetSingletonPtr()->GetMainWindow()->update();
}

void Kylin::LoadingProgress::resourceGroupLoadStarted( const Ogre::String& groupName, size_t resourceCount )
{
	assert(m_dNumGroupsLoad > 0 && "You stated you were not going to load "
		"any groups, but you did! Divide by zero would follow...");
	m_fProgressBarInc = m_fProgressBarMaxSize * (1-m_fInitProportion) / 
		(KFLOAT)resourceCount;
	m_fProgressBarInc /= m_dNumGroupsLoad;
	
	OgreRoot::GetSingletonPtr()->GetMainWindow()->update();
}

void Kylin::LoadingProgress::resourceLoadStarted( const Ogre::ResourcePtr& resource )
{
	OgreRoot::GetSingletonPtr()->GetMainWindow()->update();
}

void Kylin::LoadingProgress::resourceLoadEnded()
{
	m_pLoadingBarElement->setWidth(
		m_pLoadingBarElement->getWidth() + m_fProgressBarInc);

	OgreRoot::GetSingletonPtr()->GetMainWindow()->update();
}

void Kylin::LoadingProgress::worldGeometryStageStarted( const Ogre::String& description )
{
	m_pLoadingBarElement->setWidth(
		m_pLoadingBarElement->getWidth() + m_fProgressBarInc);

	OgreRoot::GetSingletonPtr()->GetMainWindow()->update();
}

void Kylin::LoadingProgress::worldGeometryStageEnded()
{

}