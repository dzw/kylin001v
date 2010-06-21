#pragma once

namespace Kylin
{
	class ClockingCallback
	{
	public:

		virtual KVOID EndTime(KCSTR& sClass,KCSTR& sName, KANY aUserData) = 0;
	};
}