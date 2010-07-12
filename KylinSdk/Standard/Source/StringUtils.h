#pragma once

#include <cstdarg> 

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
		KSTR curLocale = setlocale(LC_ALL, NULL);    // curLocale = "C";
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

	static KSTR format(const KSTR& fmt, ...)
	{ 
		va_list argptr; 
		va_start(argptr, fmt); 
		KINT bufsize = _vsnprintf(NULL, 0, fmt.c_str(), argptr) + 2;
		KCHAR* buf = KNEW KCHAR[bufsize];
		_vsnprintf(buf, bufsize, fmt.c_str(), argptr); 
		KSTR s(buf);
		KDEL[] buf;
		va_end(argptr);
		return s;
	}

	static KSTR replace(KCSTR &str, KCCHAR *string_to_replace, KCCHAR *new_string)
	{
		KSTR tmp = str;
		// Find the first string to replace
		int index = tmp.find(string_to_replace);
		// while there is one 
		while(index != std::string::npos)
		{ 
			// Replace it 
			tmp.replace(index, strlen(string_to_replace), new_string);
			// Find the next one 
			index = tmp.find(string_to_replace, index + strlen(new_string));
		}

		return tmp;
	}

};