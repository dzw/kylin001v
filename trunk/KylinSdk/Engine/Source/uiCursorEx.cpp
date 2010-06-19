#include "engpch.h"
#include "uiCursorEx.h"

#include <MyGUI.h>


Kylin::CursorEx::CursorEx()
: GuiBase(CLASS_TO(CursorEx))
, m_eType(CT_NORMAL)
{

}


KBOOL Kylin::CursorEx::Initialize()
{
	//MyGUI::getGUI()->load("Pointers_W32.xml");
	
	MyGUI::Gui::getInstance().load("Pointers.xml");
	
	SetPointer(CT_NORMAL);

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

	m_eType = eType;
}

Kylin::CursorType Kylin::CursorEx::GetPointerType()
{
	return m_eType;
}