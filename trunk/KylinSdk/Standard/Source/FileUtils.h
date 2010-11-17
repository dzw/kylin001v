#pragma once

#pragma warning(disable:4996)

class FileUtils
{
public:
	static KVOID SetExePath(KCSTR& path) { m_sExePath = path; } 

	static KSTR GetExePath() { return m_sExePath; }

	static KBOOL IsFileExist(KCSTR& sFile)
	{
		ifstream file(sFile.data());
		if(!file)
			return false;
		file.close();
		return true;
	}
	
	static KSTR GetAbsolutePath(KCSTR& sPath)
	{
		KCHAR drive[_MAX_DRIVE]; 
		KCHAR dir[_MAX_DIR]; 
		KCHAR fname[_MAX_FNAME]; 
		KCHAR ext[_MAX_EXT]; 
		
		_splitpath( sPath.data(), drive, dir, fname, ext );
		
		return dir;
	}
	
	static KSTR GetRelativePath(KCSTR& sPath)
	{
		KCHAR drive[_MAX_DRIVE]; 
		KCHAR dir[_MAX_DIR]; 
		KCHAR fname[_MAX_FNAME]; 
		KCHAR ext[_MAX_EXT]; 

		_splitpath( sPath.data(), drive, dir, fname, ext );

		return dir;
	}

	static KSTR GetFileName(KCSTR& sPath)
	{
		KCHAR drive[_MAX_DRIVE]; 
		KCHAR dir[_MAX_DIR]; 
		KCHAR fname[_MAX_FNAME]; 
		KCHAR ext[_MAX_EXT]; 

		_splitpath( sPath.data(), drive, dir, fname, ext );

		return fname;
	}
	
	static KSTR GetFileNameWithSuffix(KCSTR& sPath)
	{
		KCHAR drive[_MAX_DRIVE]; 
		KCHAR dir[_MAX_DIR]; 
		KCHAR fname[_MAX_FNAME]; 
		KCHAR ext[_MAX_EXT]; 

		_splitpath( sPath.data(), drive, dir, fname, ext );
		
		KSTR sName = fname;
		sName += ext;

		return sName;
	}

	static KSTR GetFileSuffix(KCSTR& sPath)
	{
		KCHAR drive[_MAX_DRIVE]; 
		KCHAR dir[_MAX_DIR]; 
		KCHAR fname[_MAX_FNAME]; 
		KCHAR ext[_MAX_EXT]; 

		_splitpath( sPath.data(), drive, dir, fname, ext );

		return ext;
	}

	static KSTR QualifyPath(KCSTR& sDir)
	{
		KSTR path = sDir;
		if(path.substr(0,1) == ".") path = m_sExePath + "/" + path;

		std::replace(path.begin(),path.end(),'\\','/');

		// Remember if there is a leading '/'
		KBOOL leadingSlash = false;
		if(path.substr(0,1) == "/")
			leadingSlash = true;

		KVEC<KSTR> list;
		KINT pos = path.find("/");
		while(pos != -1)
		{
			if(pos > 0 && path.substr(0,pos) != ".")  // Ignore zero strings and same directory pointers
				list.push_back(path.substr(0,pos));
			path.erase(0,pos + 1);
			pos = path.find("/");
		}

		if(path != "") 
			list.push_back(path);

		KUINT pos2 = 0;
		while(list.size() > pos2)
		{
			if(list[pos2] == "..")
			{
				list.erase(list.begin() + pos2 - 1,list.begin() + pos2 + 1);
				pos2--;
			}
			else
				pos2++;
		}

		if(list.size() == 0) 
			return "";

		path = list[0];

		for(KUINT i = 1;i < list.size();i++)
		{ 
			path += "/" + list[i];
		}

		if(leadingSlash)
			path = "/" + path;

		return path;
	}
	//------------------------------------------------------------------
	static KVOID GetFileList(KSTR path, KStrVector &list,KBOOL bSearchSubDir = false);
	
	protected:
		static KSTR m_sExePath;
};

