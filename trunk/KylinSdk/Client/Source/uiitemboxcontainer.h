#pragma once

#include <MyGUI.h>
#include "uiItemBoxMenu.h"


namespace Kylin
{
	class ItemBoxContainer : public wraps::BaseLayout
	{
	public:
		ItemBoxContainer(const KSTR& _layout);
		ItemBoxMenu * getItemBox() { return mItemBox; }

	private:
		ItemBoxMenu * mItemBox;
	};

}
