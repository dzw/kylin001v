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
 
	static KSTR ws2s(const KWSTR& ws)
	{
		KSTR curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
		setlocale(LC_ALL, "chs");
		const wchar_t* _Source = ws.c_str();
		size_t _Dsize = 2 * ws.size() + 1;
		KCHAR *_Dest = KNEW KCHAR[_Dsize];
		memset(_Dest,0,_Dsize);
		wcstombs(_Dest,_Source,_Dsize);
		KSTR result = _Dest;
		KDEL []_Dest;
		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}

	static KWSTR s2ws(KCSTR& s)
	{
		setlocale(LC_ALL, "chs"); 
		const KCHAR* _Source = s.c_str();
		size_t _Dsize = s.size() + 1;
		wchar_t *_Dest = KNEW wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs(_Dest,_Source,_Dsize);
		KWSTR result = _Dest;
		KDEL []_Dest;
		setlocale(LC_ALL, "C");
		return result;
	}

};