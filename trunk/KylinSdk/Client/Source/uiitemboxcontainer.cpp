
#include "cltpch.h"
#include "uiItemBoxContainer.h"

namespace Kylin
{

	ItemBoxContainer::ItemBoxContainer(const KSTR& _layout) : wraps::BaseLayout(_layout)
	{
		assignBase(mItemBox, "box_Items");
	}

}
