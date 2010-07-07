#pragma once

struct DamageUnit
{
	DamageUnit(KUINT uLevel, KUINT uMinValue, KUINT uMaxValue, KUINT uType)
		: mLevel(uLevel)
		, mMinValue(uMinValue)
		, mMaxValue(uMaxValue)
		, mType(uType)
	{
	}

	KUINT mLevel;
	KUINT mMinValue;
	KUINT mMaxValue;
	KUINT mType;
};

namespace Kylin
{

	class DamageSystem
	{
	public:

		static KINT Calculate(const DamageUnit& kDamage, KUINT uEntityID);
		

	};
}