#pragma once

#include <MyGUI.h>
#include "ItemBox/BaseCellView.h"
#include "uiItemMenu.h"

namespace Kylin
{
	class CellView : public wraps::BaseCellView<ItemMenu>
	{
	public:
		CellView(MyGUI::Widget* _parent);

		void update(const MyGUI::IBDrawItemInfo & _info, ItemMenu * _data);
		static void getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord & _coord, bool _drop);

	private:
		MyGUI::StaticImage* mImageBack;
		MyGUI::StaticImage* mImageBorder;
		MyGUI::StaticImage* mImageItem;
// 		MyGUI::StaticText* mTextBack;
// 		MyGUI::StaticText* mTextFront;

	};
}
