#include "cltpch.h"
#include "uiShortcutMenu.h"

#include "ClSceneLoader.h"
#include "KylinRoot.h"
#include "Scene.h"
#include "PlayerController.h"

Kylin::ShortcutMenu::ShortcutMenu()
: GuiBase(CLASS_TO(ShortcutMenu))
{
	initialiseByAttributes(this);
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
	
	m_kImageHealthCoord.x = info.frames[0].left;
	m_kImageHealthCoord.y = info.frames[0].top;
	m_kImageHealthCoord.z = info.size.width;
	m_kImageHealthCoord.w = info.size.height;

	m_pImageExprience->setItemResourcePtr(resource_exp);
	m_pImageExprience->setItemGroup("States");
	
	//-----------------------------------------------------

	m_pImageHealth->setColour(MyGUI::Colour(0.85,0,0,0.1));
	
	//-----------------------------------------------------
	// test code
	
	SetWidgetWidthPct("image_experience",0.6f);
	
	//-----------------------------------------------------
	m_pImageSkill_L->eventMouseButtonPressed = newDelegate(this, &ShortcutMenu::NotifyClick_Skill);
	m_pImageSkill_1->eventMouseButtonPressed = newDelegate(this, &ShortcutMenu::NotifyClick_Skill);
	m_pImageSkill_2->eventMouseButtonPressed = newDelegate(this, &ShortcutMenu::NotifyClick_Skill);

	return true;
}

KVOID Kylin::ShortcutMenu::Destroy()
{

}
//-----------------------------------------------------------------------------
KVOID Kylin::ShortcutMenu::SetVisible( KBOOL bVisible )
{
	
}
//-----------------------------------------------------------------------------
KVOID Kylin::ShortcutMenu::SetHPWidthPct(KFLOAT fH )
{	
	if ( fH >= 1.0f)
	{
		MyGUI::ResourceImageSetPtr resource_health = nullptr;
		resource_health = MyGUI::ResourceManager::getInstance().getByName("pic_health")->castType<MyGUI::ResourceImageSet>();
		m_pImageHealth->setItemResourcePtr(resource_health);
		m_pImageHealth->setItemGroup("States");
	}

	KINT h = m_kImageHealthCoord.w * fH;
	KINT t = m_kImageHealthCoord.y + (1-fH) * m_kImageHealthCoord.w;
	m_pImageHealth->setImageCoord(MyGUI::IntCoord(m_kImageHealthCoord.x,t,m_kImageHealthCoord.z,h));

	MyGUI::IntCoord crd = m_pImageHealth->getCoord();
	
	crd.top = t;//(1-fH) * m_kImageHealthCoord.w + 0.5f;
	crd.height = h;

	m_pImageHealth->setCoord(crd);
}
//-----------------------------------------------------------------------------
KVOID Kylin::ShortcutMenu::SetWidgetWidthPct( KSTR sName, KFLOAT fW )
{
	if (sName == "image_experience")
	{
		m_pImageExprience->setSize(int(fW*m_pImageExprience->getWidth()),m_pImageExprience->getHeight());
	}
}

//-----------------------------------------------------------------------------
KVOID Kylin::ShortcutMenu::SetSkillInfo( KSTR sIcon, KCHAR cP, KUINT uActionID )
{
	MyGUI::ResourceImageSetPtr resource_img = nullptr;
	resource_img = MyGUI::ResourceManager::getInstance().getByName(sIcon)->castType<MyGUI::ResourceImageSet>();
	
	MyGUI::StaticImage* pImage = NULL;

	switch(cP)
	{
	case 'l':
		pImage = m_pImageSkill_L;
		break;
	case 'r':
		pImage = m_pImageSkill_R;
		break;
	case '1':
		pImage = m_pImageSkill_1;
		break;
	case '2':
		pImage = m_pImageSkill_2;
		break;
	case '3':
		
		break;
	case '4':

		break;
	case '5':

		break;
	}

	pImage->setItemResourcePtr(resource_img);
	pImage->setItemGroup("States");
	//------------------------------------------------------------------
	m_kActionShortcutMap.insert(std::pair<KCHAR,KUINT>(cP,uActionID));
}

KVOID Kylin::ShortcutMenu::NotifyClick_Skill( MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id )
{
	// test code skill
	ClSceneLoader* pLoader = (ClSceneLoader*)KylinRoot::GetSingletonPtr()->GetCurrentScene()->GetSceneLoader();
	if (pLoader)
	{
		KCHAR cKey = 'l';

		if ( _sender == m_pImageSkill_1)
			cKey = '1';
		else if ( _sender == m_pImageSkill_2)
			cKey = '2';
// 		else if ( _sender == m_pImageSkill_2)
// 			cKey = '3';
// 		else if ( _sender == m_pImageSkill_2)
// 			cKey = '4';
// 		else if ( _sender == m_pImageSkill_2)
// 			cKey = '5';

		KMAP<KCHAR, KUINT>::iterator it = m_kActionShortcutMap.find(cKey);
		if ( it != m_kActionShortcutMap.end())
		{
			pLoader->GetController()->UseSkill(it->second);
		}
	}
}
//-----------------------------------------------------------------------------