#include "engpch.h"
#include "uiFrameStats.h"
#include "rOgreRoot.h"

//////////////////////////////////////////////////////////////////////////
//
Kylin::DebugFrameStats::DebugFrameStats()
: m_pFrameStats(NULL)
{

}

Kylin::DebugFrameStats::~DebugFrameStats()
{
	Destroy();
}

KBOOL Kylin::DebugFrameStats::Initialize()
{
	m_sGuiName = "DebugFrameStats";

	//////////////////////////////////////////////////////////////////////////
	const KSTR sLayer = "Statistic";
	if ( ! MyGUI::LayerManager::getInstance().isExist(sLayer)) 
		return false;

	const KSTR sSkin = "StatisticRect";
	if ( ! MyGUI::SkinManager::getInstance().isExist(sSkin)) 
		return false;

	m_pFrameStats = MyGUI::Gui::getInstance().createWidget<MyGUI::StaticText>(sSkin, MyGUI::IntCoord(), MyGUI::Align::Default, sLayer);
	//////////////////////////////////////////////////////////////////////////

	SetVisible(true);

	return true;
}

KVOID Kylin::DebugFrameStats::Render( KFLOAT fElapsed )
{
	if (m_pFrameStats)
	{
		static float time = 0;
		time += fElapsed;
		if (time > 1)
		{
			time -= 1;
			try
			{
				const Ogre::RenderTarget::FrameStats& stats = OgreRoot::GetSingletonPtr()->GetSystemStats();

				std::ostringstream stream;

				stream << "FPS" << " : " << MyGUI::utility::toString<int>((int)stats.avgFPS);
				stream << "\n";
				stream << "Triangle" << " : " << MyGUI::utility::toString<int>((int)stats.triangleCount);
				stream << "\n";
				stream << "Batch" << " : " << MyGUI::utility::toString<int>((int)stats.batchCount);
				stream << "\n";

				m_pFrameStats->setCaption(stream.str());

				MyGUI::ISubWidgetText * text = m_pFrameStats->getSubWidgetText();
				if (text)
				{
					const MyGUI::IntSize& size = text->getTextSize() + m_pFrameStats->getSize() - text->getSize();
					const MyGUI::IntSize& size_view = MyGUI::Gui::getInstance().getViewSize();
					m_pFrameStats->setCoord(size_view.width - size.width - 20, size_view.height - size.height - 20, size.width, size.height);
				}
			}
			catch (...)
			{
			}
		}
	}
}

KVOID Kylin::DebugFrameStats::Destroy()
{
	if (m_pFrameStats)
	{
		MyGUI::Gui::getInstance().destroyChildWidget(m_pFrameStats);
		m_pFrameStats = NULL;
	}
}

KVOID Kylin::DebugFrameStats::SetVisible( KBOOL bVisible )
{
	GuiBase::SetVisible(bVisible);

	m_pFrameStats->setVisible(bVisible);
}

