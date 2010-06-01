#pragma once

class StringUtils
{
public:
	static KVOID Split(KSTR str, KSTR delim, KVEC<KSTR>& results)
	{
		KINT cutAt;
		while( (cutAt = str.find_first_of(delim)) != str.npos )
		{
			if(cutAt > 0)
			{
				results.push_back(str.substr(0,cutAt));
			}
			str = str.substr(cutAt+1);
		}
		if(str.length() > 0)
		{
			results.push_back(str);
		}
	}
 
};