#include "engpch.h"
#include "uiCursorEx.h"

#include <MyGUI.h>
#include "rogreRoot.h"
#include "InputManager.h"


Kylin::CursorEx::CursorEx()
: GuiBase(CLASS_TO(CursorEx))
, m_eType(CT_NORMAL)
{

}


KBOOL Kylin::CursorEx::Initialize()
{

	//MyGUI::PointerManager::getInstance().load("Pointers.xml");
	
	SetPointer(CT_NORMAL);

	MyGUI::IntSize size = MyGUI::Gui::getInstance().getViewSize();
	Kylin::OgreRoot::GetSingletonPtr()->GetInputManager()->SetMousePosition(size.width / 2, size.height / 2);

	return true;
}

KVOID Kylin::CursorEx::SetVisible(KBOOL bShow)
{
	GuiBase::SetVisible(bShow);

	MyGUI::PointerManager& refPointer = MyGUI::PointerManager::getInstance();
	if ( bShow )
	{
		if (!refPointer.isVisible())
			refPointer.setVisible(bShow);
	}
	else
	{
		if (refPointer.isVisible())
			refPointer.setVisible(bShow);
	}
}

KVOID Kylin::CursorEx::SetPointer( CursorType eType )
{
	MyGUI::PointerManager& refPointer = MyGUI::PointerManager::getInstance();

	switch (eType)
	{
	case CT_NORMAL:
		
		MyGUI::PointerManager::getInstance().setPointer("ptr_Normal");

		break;
	case CT_ATTACK:
		
		MyGUI::PointerManager::getInstance().setPointer("ptr_Attack");

		break;
	case CT_AUTORUN:

		break;
	case CT_UNREACHABLE:

		break;
	}
	
	MyGUI::PointerManager::getInstance().setVisible(true);
	m_eType = eType;
}

Kylin::CursorEx::CursorType Kylin::CursorEx::GetPointerType()
{
	return m_eType;
}