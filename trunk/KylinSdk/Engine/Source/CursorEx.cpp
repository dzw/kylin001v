#include "engpch.h"
#include "CursorEx.h"

#include <MyGUI.h>

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

