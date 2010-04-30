#pragma once

#include "Standard.h"

namespace Kylin
{

	template <typename _Key, typename _Base, typename _Predicator = std::less<_Key> >
	class CClassFactory
	{
	public:
		CClassFactory() {};
		~CClassFactory() {};

		typedef _Base* (*CreatorFunction) (KVOID);
		typedef std::map<_Key, CreatorFunction, _Predicator> _mapFactory;

		// called at the beginning of execution to register creation functions
		static _Key RegisterCreatorFunction(_Key idKey, CreatorFunction classCreator)
		{
			assert(get_mapFactory()->find(idKey) == get_mapFactory()->end() && "Given key had already been registered\n");
			get_mapFactory()->insert(std::pair<_Key, CreatorFunction>(idKey, classCreator));
			return idKey;
		}

		// Tries to create instance based on the key
		// using creator function (if provided)
		static _Base* CreateInstance(_Key idKey)
		{
			_mapFactory::iterator it = get_mapFactory()->find(idKey);
			if (it != get_mapFactory()->end())
			{
				if (it->second)
				{
					return it->second();
				}
			}
			return NULL;
		}

		//used for shutdown factory
		static KVOID Clear(){
			get_mapFactory()->clear();
		}
		//*****************************

	protected:
		// Map where the construction info is stored.
		// To prevent inserting into map before initialisation takes place,
		// actual map is a static member, so it will be initialised
		// at the first call
		static _mapFactory * get_mapFactory()
		{
			static _mapFactory m_sMapFactory;
			return &m_sMapFactory;
		}
	};

}