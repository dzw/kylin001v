#include "cltpch.h"
#include "uiItemBoxMenu.h"

namespace Kylin
{

	ItemBoxMenu::ItemBoxMenu(MyGUI::Widget* _parent) : wraps::BaseItemBox<CellView>("", _parent)
	{
	}

	ItemBoxMenu::~ItemBoxMenu()
	{
		MyGUI::ItemBox* box = getItemBox();
		size_t count = box->getItemCount();
		for (size_t pos=0; pos<count; ++pos)
		{
			delete *box->getItemDataAt<ItemMenu*>(pos);
		}
	}

}
