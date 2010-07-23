#pragma once

#include "BaseLayout/BaseLayout.h"
#include "GuiManager.h"
#include "InputListener.h"
#include "Kitbag.h"


namespace Kylin
{
	ATTRIBUTE_CLASS_LAYOUT(MiniMapMenu, "minimap.layout");
	class MiniMapMenu : public GuiBase
		, public InputListener
		, public wraps::BaseLayout

	{
	public:
		MiniMapMenu();

		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed);
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);
		
		KVOID	SetMyself(KUINT uID){ m_uMyselfID = uID; }
		KVOID	SetBoss(KUINT uID) { m_uBossID = uID; }

	protected:
		virtual KVOID OnKeyDown(KUINT uKey);
		
		KPoint2 MiniCoords(const KPoint3& ptSrc);
		KVOID	Radar(KFLOAT fElapsed);

	protected:
		ATTRIBUTE_FIELD_WIDGET_NAME(MiniMapMenu, m_pImageBack, "_Main");
		MyGUI::StaticImage* m_pImageBack;
		ATTRIBUTE_FIELD_WIDGET_NAME(MiniMapMenu, m_pPlayerMark, "player_mark");
		MyGUI::StaticImage* m_pPlayerMark;
		ATTRIBUTE_FIELD_WIDGET_NAME(MiniMapMenu, m_pBossMark, "boss_mark");
		MyGUI::StaticImage* m_pBossMark;
	
	private:
		
		KFLOAT		m_fTmpTime;

		KUINT		m_uMyselfID;
		KUINT		m_uBossID;
		
	};
}