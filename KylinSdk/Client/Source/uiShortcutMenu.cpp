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
	SetWidgetHeightPct("image_health",0.8f);
	
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
KVOID Kylin::ShortcutMenu::SetWidgetHeightPct( KSTR sName, KFLOAT fH )
{	
	if (sName == "image_health")
	{
		KINT h = m_kImageHealthCoord.w * fH;
		KINT t = m_kImageHealthCoord.y + (1-fH) * m_kImageHealthCoord.w;
		m_pImageHealth->setImageCoord(MyGUI::IntCoord(m_kImageHealthCoord.x,t,m_kImageHealthCoord.z,h));

		MyGUI::IntCoord crd = m_pImageHealth->getCoord();
		
		crd.top += (1-fH) * m_kImageHealthCoord.w + 0.5f;
		crd.height = h;

		m_pImageHealth->setCoord(crd);
	}
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
KVOID Kylin::ShortcutMenu::SetSkillIcon( KSTR sIcon,KCHAR cP )
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

}

KVOID Kylin::ShortcutMenu::NotifyClick_Skill( MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id )
{
	// test code skill
	ClSceneLoader* pLoader = (ClSceneLoader*)KylinRoot::GetSingletonPtr()->GetCurrentScene()->GetSceneLoader();
	if (pLoader)
	{
		if ( _sender == m_pImageSkill_L )
			pLoader->GetController()->UseSkill(4);
		else if ( _sender == m_pImageSkill_1)
			pLoader->GetController()->UseSkill(5);
		else if ( _sender == m_pImageSkill_2)
			pLoader->GetController()->UseSkill(6);
	}
}
//-----------------------------------------------------------------------------