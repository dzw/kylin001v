#include "cltpch.h"
#include "uiShortcutMenu.h"
#include "uiTipsMenu.h"

#include "ClSceneLoader.h"
#include "KylinRoot.h"
#include "Scene.h"
#include "PlayerController.h"

Kylin::ShortcutMenu::ShortcutMenu()
: GuiBase(CLASS_TO(ShortcutMenu))
{
	initialiseByAttributes(this);

	m_pActionTips = KNEW TipsMenu();
	m_pActionTips->Hide();
}

KBOOL Kylin::ShortcutMenu::Initialize()
{
	MyGUI::ResourceImageSetPtr resource_back = nullptr;
	resource_back = MyGUI::ResourceManager::getInstance().getByName("pic_shortcut_bar")->castType<MyGUI::ResourceImageSet>();
	
	MyGUI::ResourceImageSetPtr resource_health = nullptr;
	resource_health = MyGUI::ResourceManager::getInstance().getByName("pic_health")->castType<MyGUI::ResourceImageSet>();

	MyGUI::ResourceImageSetPtr resource_exp = nullptr;
	resource_exp = MyGUI::ResourceManager::getInstance().getByName("pic_exp")->castType<MyGUI::ResourceImageSet>();

	//-----------------------------------------------------
	m_pImageBack->setItemResourcePtr(resource_back);
	m_pImageBack->setItemGroup("States");

	m_pImageHealth->setItemResourcePtr(resource_health);
	m_pImageHealth->setItemGroup("States");

	MyGUI::ImageIndexInfo info = resource_health->getIndexInfo("States","Normal");
	
	m_kImageHealthCoord.left	= info.frames[0].left;
	m_kImageHealthCoord.top		= info.frames[0].top;
	m_kImageHealthCoord.width	= info.size.width;
	m_kImageHealthCoord.height	= info.size.height;

	m_kHealthWidgetCoord		= m_pImageHealth->getCoord();
	
	m_pExprienceWidgetCoord		= m_pImageExprience->getCoord();

	m_pImageExprience->setItemResourcePtr(resource_exp);
	m_pImageExprience->setItemGroup("States");
	
	//-----------------------------------------------------

	m_pImageHealth->setColour(MyGUI::Colour(0.85,0,0,0.1));
	
	//-----------------------------------------------------
	m_pImageSkill_L->eventMouseButtonPressed = newDelegate(this, &ShortcutMenu::NotifyClick_Skill);
	m_pImageSkill_1->eventMouseButtonPressed = newDelegate(this, &ShortcutMenu::NotifyClick_Skill);
	m_pImageSkill_2->eventMouseButtonPressed = newDelegate(this, &ShortcutMenu::NotifyClick_Skill);
	m_pImageSkill_3->eventMouseButtonPressed = newDelegate(this, &ShortcutMenu::NotifyClick_Skill);
	m_pImageSkill_4->eventMouseButtonPressed = newDelegate(this, &ShortcutMenu::NotifyClick_Skill);
	m_pImageSkill_5->eventMouseButtonPressed = newDelegate(this, &ShortcutMenu::NotifyClick_Skill);

	m_pImageSkill_L->eventToolTip	 = newDelegate(this, &ShortcutMenu::NotifyToolTip);
	m_pImageSkill_1->eventToolTip	 = newDelegate(this, &ShortcutMenu::NotifyToolTip);
	m_pImageSkill_2->eventToolTip	 = newDelegate(this, &ShortcutMenu::NotifyToolTip);
	m_pImageSkill_3->eventToolTip	 = newDelegate(this, &ShortcutMenu::NotifyToolTip);
	m_pImageSkill_4->eventToolTip	 = newDelegate(this, &ShortcutMenu::NotifyToolTip);
	m_pImageSkill_5->eventToolTip	 = newDelegate(this, &ShortcutMenu::NotifyToolTip);

	SetExpWidthPct(.0f);

	return true;
}

KVOID Kylin::ShortcutMenu::Destroy()
{
	SAFE_DEL(m_pActionTips);
}
//-----------------------------------------------------------------------------
KVOID Kylin::ShortcutMenu::SetVisible( KBOOL bVisible )
{
	
}
//-----------------------------------------------------------------------------
KVOID Kylin::ShortcutMenu::SetHPWidthPct(KFLOAT fH )
{	
//	if (fH >= 1.0f )
	{
//		m_pImageHealth->setCoord(m_kHealthWidgetCoord);
//		m_pImageHealth->setImageCoord(m_kImageHealthCoord);
	}
//	else if (fH >=.0f)
	{
 		KINT h = (KFLOAT)m_kHealthWidgetCoord.height * fH;
// 		KINT t = (KFLOAT)m_kImageHealthCoord.top + (1.0f-fH) * (KFLOAT)m_kImageHealthCoord.height;
// 		m_pImageHealth->setImageCoord(MyGUI::IntCoord(m_kImageHealthCoord.left,t,m_kImageHealthCoord.width,h));
// 
 		MyGUI::IntCoord crd = m_kHealthWidgetCoord;
 
 		crd.top +=  (1.0f-fH) * (KFLOAT)m_kImageHealthCoord.height;
 		crd.height = h;
 
 		m_pImageHealth->setCoord(crd);
	}
}
//-----------------------------------------------------------------------------
KVOID Kylin::ShortcutMenu::SetExpWidthPct( KFLOAT fW )
{
	m_pImageExprience->setSize(int(fW*m_pExprienceWidgetCoord.width),m_pImageExprience->getHeight());
}

//-----------------------------------------------------------------------------
KVOID Kylin::ShortcutMenu::SetSkillInfo( KSTR sIcon, KFLOAT fCooldown, KUINT uActionID, KBOOL bDefault,KCSTR& sTitle, KCSTR& sContent )
{
	MyGUI::ResourceImageSetPtr resource_img = nullptr;
	resource_img = MyGUI::ResourceManager::getInstance().getByName(sIcon)->castType<MyGUI::ResourceImageSet>();
	
	//------------------------------------------------------------------
	if (bDefault)
	{
		m_pImageSkill_L->setItemResourcePtr(resource_img);
		m_pImageSkill_L->setItemGroup("States");

		m_kActions[0].pImage	= m_pImageSkill_L;
		m_kActions[0].fCooldown = fCooldown;
		m_kActions[0].fInitCooldown = fCooldown;
		m_kActions[0].uActionID = uActionID;
		m_kActions[0].sTitle	= sTitle;
		m_kActions[0].sContent	= sContent;
	}
	else
	{
		MyGUI::StaticImage* pImage = NULL;
		KUINT i = HasSpare();
		if (i > 0)
		{
			switch(i)
			{
			case 1:
				pImage = m_pImageSkill_1;
				break;
			case 2:
				pImage = m_pImageSkill_2;
				break;
			case 3:
				pImage = m_pImageSkill_3;
				break;
			case 4:
				pImage = m_pImageSkill_4;
				break;
			case 5:
				pImage = m_pImageSkill_5;
				break;
			}

			pImage->setItemResourcePtr(resource_img);
			pImage->setItemGroup("States");

			m_kActions[i].pImage	= m_pImageSkill_L;
			m_kActions[i].fCooldown = fCooldown;
			m_kActions[i].fInitCooldown = fCooldown;
			m_kActions[i].uActionID = uActionID;
			m_kActions[i].sTitle	= sTitle;
			m_kActions[i].sContent	= sContent;
		}
	}
}

KVOID Kylin::ShortcutMenu::NotifyClick_Skill( MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id )
{
	// test code skill
	ClSceneLoader* pLoader = (ClSceneLoader*)KylinRoot::GetSingletonPtr()->GetCurrentScene()->GetSceneLoader();
	if (pLoader && _id == MyGUI::MouseButton::Left)
	{
		KUINT uKey = -1;

		if ( _sender == m_pImageSkill_1)
			uKey = 1;
		else if ( _sender == m_pImageSkill_2)
			uKey = 2;
		else if ( _sender == m_pImageSkill_3)
			uKey = 3;
		else if ( _sender == m_pImageSkill_4)
			uKey = 4;
		else if ( _sender == m_pImageSkill_5)
			uKey = 5;

		if ( m_kActions[uKey].uActionID != INVALID_ID && 
			 m_kActions[uKey].fCooldown <= .0f )
		{
			pLoader->GetController()->UseSkill(m_kActions[uKey].uActionID);

			m_kActions[uKey].pImage->setAlpha(0.6f);

			m_kActions[uKey].fCooldown = m_kActions[uKey].fInitCooldown;
		}
	}
}

KVOID Kylin::ShortcutMenu::Render( KFLOAT fElapsed )
{
	UpdateCooldown(fElapsed);
}

KVOID Kylin::ShortcutMenu::UpdateCooldown( KFLOAT fElapsed )
{
	for (KUINT i = 1; i < 6; i++)
	{
		if ( m_kActions[i].uActionID != INVALID_ID && 
			 m_kActions[i].fCooldown > .0f )
		{
			m_kActions[i].fCooldown -= fElapsed;
			if (m_kActions[i].fCooldown <= .0f)
			{
				m_kActions[i].pImage->setAlpha(1.0f);
			}
		}
	}
}

KUINT Kylin::ShortcutMenu::HasSpare()
{
	for (KUINT i = 1; i < 6; i++)
	{
		if (m_kActions[i].uActionID == INVALID_ID)
		{
			return i;
		}
	}

	return -1;
}

KVOID Kylin::ShortcutMenu::NotifyToolTip( MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info  )
{
	if (_info.type == MyGUI::ToolTipInfo::Show)
	{
		KUINT uKey = -1;

		if ( _sender == m_pImageSkill_1)
			uKey = 1;
		else if ( _sender == m_pImageSkill_2)
			uKey = 2;
		else if ( _sender == m_pImageSkill_3)
			uKey = 3;
		else if ( _sender == m_pImageSkill_4)
			uKey = 4;
		else if ( _sender == m_pImageSkill_5)
			uKey = 5;
		else if ( _sender == m_pImageSkill_L)
			uKey = 0;

		if ( m_kActions[uKey].uActionID != INVALID_ID )
		{
			if (!m_kActions[uKey].sContent.empty())
			{
				m_pActionTips->SetTitle(m_kActions[uKey].sTitle);
				m_pActionTips->SetContent(m_kActions[uKey].sContent);
				m_pActionTips->Show(_info.point);
			}
		}
	}
	else if (_info.type == MyGUI::ToolTipInfo::Hide)
	{
		m_pActionTips->Hide();
	}
}

KVOID Kylin::ShortcutMenu::OnKeyDown( KUINT uKey )
{
	KUINT uIndex = -1;

	uIndex = uKey - MyGUI::KeyCode::One + 1;
	
	if (uIndex < 1 || uIndex > 5) return;

	ClSceneLoader* pLoader = (ClSceneLoader*)KylinRoot::GetSingletonPtr()->GetCurrentScene()->GetSceneLoader();
	if ( m_kActions[uIndex].uActionID != INVALID_ID && 
		m_kActions[uIndex].fCooldown <= .0f )
	{
		pLoader->GetController()->UseSkill(m_kActions[uIndex].uActionID);

		m_kActions[uIndex].pImage->setAlpha(0.3f);

		m_kActions[uIndex].fCooldown = m_kActions[uIndex].fInitCooldown;
	}
}
//-----------------------------------------------------------------------------