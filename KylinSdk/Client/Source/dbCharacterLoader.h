#pragma once

#include "DataLoader.h"

#define DB_CHARACTER "Data\\db\\char.csv"

namespace Kylin
{
	class CharacterLoader : public DataLoader
	{
	public:
		CharacterLoader::CharacterLoader()
			: DataLoader(DB_CHARACTER)
		{

		}
	protected:
		virtual KVOID OnFinish(){}
	};
}