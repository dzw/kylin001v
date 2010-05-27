#pragma once

#include <MyGUI.h>
#include "ItemBox/BaseItemBox.h"
#include "uiCellView.h"

namespace Kylin
{

	class ItemBoxMenu : public wraps::BaseItemBox<CellView>
	{
	public:
		ItemBoxMenu(MyGUI::Widget* _parent);
		virtual ~ItemBoxMenu();

	};

}

