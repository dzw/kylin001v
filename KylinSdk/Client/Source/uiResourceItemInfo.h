#pragma once

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_GenericFactory.h"

namespace Kylin
{

	class ResourceItemInfo;
	typedef ResourceItemInfo * ResourceItemInfoPtr;

	class ResourceItemInfo :
		public MyGUI::IResource,
		public MyGUI::GenericFactory<ResourceItemInfo>
	{
		friend class MyGUI::GenericFactory<ResourceItemInfo>;

		MYGUI_RTTI_DERIVED( ResourceItemInfo )

	private:
		ResourceItemInfo() { }
		virtual ~ResourceItemInfo() { }

		virtual KVOID deserialization(MyGUI::xml::ElementPtr _node, MyGUI::Version _version);

	public:
		const KSTR& getItemName() { return mItemName; }
		const KSTR& getItemDescription() { return mItemDescription; }
		const KSTR& getItemResourceImage() { return mItemResourceImage; }

	private:
		KSTR mItemName;
		KSTR mItemDescription;
		KSTR mItemResourceImage;
	};
}

