#pragma once

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace Kylin
{
	class ItemMenu;
	class TipMenu : public wraps::BaseLayout
	{
	public:
		TipMenu();

		void Show(ItemMenu * _data, const MyGUI::IntPoint & _point);
		void Hide();

	private:

		MyGUI::StaticText* mTextName;
		MyGUI::StaticText* mTextCount;
		MyGUI::Edit* mTextDesc;
		//MyGUI::StaticImage* mImageInfo;

		int mOffsetHeight;
	};

} 


