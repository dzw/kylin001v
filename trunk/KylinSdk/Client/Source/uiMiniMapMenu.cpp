#include "cltpch.h"
#include "uiMiniMapMenu.h"
#include "Character.h"
#include "KylinRoot.h"


Kylin::MiniMapMenu::MiniMapMenu()
: GuiBase(CLASS_TO(MiniMapMenu))
, m_uMyselfID(INVALID_ID)
, m_fTmpTime(.0f)
{
	initialiseByAttributes(this);
}

KBOOL Kylin::MiniMapMenu::Initialize()
{
	
	m_pBossMark->setColour(MyGUI::Colour(0.85,0,0,0.1));

	SetVisible(false);

	return true;
}

KVOID Kylin::MiniMapMenu::Destroy()
{

}

KVOID Kylin::MiniMapMenu::SetVisible( KBOOL bVisible )
{
	mMainWidget->setVisible(bVisible);

	GuiBase::SetVisible(bVisible);
}

KVOID Kylin::MiniMapMenu::OnKeyDown( KUINT uKey )
{
	if (uKey == MyGUI::KeyCode::Tab)
	{
		if (this->IsVisible())
			SetVisible(false);
		else
			SetVisible(true);
	}
}

KVOID Kylin::MiniMapMenu::Render( KFLOAT fElapsed )
{
	if (m_fTmpTime < .2f)
	{
		m_fTmpTime += fElapsed;
		if (m_fTmpTime >= .2f)
		{
			Radar(fElapsed);
			m_fTmpTime = .0f;
		}		
	}
}

const int g_world_size = 1024;
KPoint2 Kylin::MiniMapMenu::MiniCoords( const KPoint3& ptSrc )
{
	KPoint2 ptNew;
	ptNew.x = (ptSrc.x + g_world_size * 0.5f) * m_pImageBack->getSize().width / g_world_size;
	ptNew.y = (ptSrc.z + g_world_size * 0.5f) * m_pImageBack->getSize().height / g_world_size;

	return ptNew;
}

KVOID Kylin::MiniMapMenu::Radar( KFLOAT fElapsed )
{
	Character* pChar = BtDynamicCast(Character,KylinRoot::GetSingletonPtr()->GetEntity(m_uMyselfID));
	if (pChar && !pChar->IsDead())
	{
		KPoint2 pt = MiniCoords(pChar->GetTranslate());
		m_pPlayerMark->setPosition(pt.x,pt.y);
	}

	pChar = BtDynamicCast(Character,KylinRoot::GetSingletonPtr()->GetEntity(m_uBossID));
	if (pChar)
	{
		KPoint2 pt = MiniCoords(pChar->GetTranslate());
		m_pBossMark->setPosition(pt.x,pt.y);
		
		m_pBossMark->setVisible(true);
	}
	else
	{
		m_pBossMark->setVisible(false);
	}

}