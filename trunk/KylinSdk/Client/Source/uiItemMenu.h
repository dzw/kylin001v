#pragma once

#include "uiResourceItemInfo.h"
#include "MyGUI_ResourceManager.h"

namespace Kylin
{
	class ItemMenu
	{
	public:
		ItemMenu() :
		  count(0),
			  mResourceInfo(nullptr),
			  mResourceImage(nullptr)
		  {
		  }

		  ItemMenu(const KSTR& _resource, size_t _count) :
		  count(_count),
			  mResourceInfo(nullptr),
			  mResourceImage(nullptr)
		  {
			  MyGUI::ResourceManager & manager = MyGUI::ResourceManager::getInstance();
			  mResourceInfo = manager.getByName(_resource)->castType<ResourceItemInfo>();
			  mResourceImage = manager.getByName(mResourceInfo->getItemResourceImage())->castType<MyGUI::ResourceImageSet>();
		  }

		  KBOOL isEmpty() const
		  {
			  return mResourceInfo == 0;
		  }

		  KBOOL compare(ItemMenu * _data) const
		  {
			  return mResourceInfo == _data->mResourceInfo;
		  }

		  KVOID add(ItemMenu * _data)
		  {
			  count += _data->count;
			  mResourceInfo = _data->mResourceInfo;
			  mResourceImage = _data->mResourceImage;
		  }

		  KVOID clear()
		  {
			  mResourceInfo = nullptr;
			  mResourceImage = nullptr;
			  count = 0;
		  }

		  size_t getCount() const { return count; }
		  ResourceItemInfoPtr getInfo() { return mResourceInfo; }
		  MyGUI::ResourceImageSetPtr getImage() { return mResourceImage; }

	private:
		size_t count;
		ResourceItemInfoPtr mResourceInfo;
		MyGUI::ResourceImageSetPtr mResourceImage;

	};
}