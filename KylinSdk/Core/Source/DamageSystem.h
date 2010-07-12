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

struct DamageResult
{
	DamageResult() : mDamage(-1)
				   , mDIFF(-1)
	{
	}

	KINT mDamage;
	KINT mDIFF;
};


namespace Kylin
{

	class DamageSystem
	{
	public:

		static DamageResult Calculate(const DamageUnit& kDamage, KUINT uEntityID);
		

	};
}