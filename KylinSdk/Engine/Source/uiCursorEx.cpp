#include "engpch.h"
#include "uiCursorEx.h"

#include <MyGUI.h>

KBOOL Kylin::CursorEx::Initialize()
{
	//MyGUI::getGUI()->load("Pointers_W32.xml");
	
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
	switch (eType)
	{
	case CT_NORMAL:

		break;
	case CT_ATTACK:

		break;
	case CT_AUTORUN:

		break;
	case CT_UNREACHABLE:

		break;
	}
}

