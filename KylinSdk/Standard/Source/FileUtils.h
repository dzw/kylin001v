#pragma once

#pragma warning(disable:C4996)

class FileUtils
{
public:
	static KBOOL IsFileExist(KSTR sFile)
	{
		ifstream file(sFile.data());
		if(!file)
			return false;
		file.close();
		return true;
	}
	
	static KSTR GetAbsolutePath(KSTR sPath)
	{
		KCHAR drive[_MAX_DRIVE]; 
		KCHAR dir[_MAX_DIR]; 
		KCHAR fname[_MAX_FNAME]; 
		KCHAR ext[_MAX_EXT]; 
		
		_splitpath( sPath.data(), drive, dir, fname, ext );
		
		return dir;
	}
	
	static KSTR GetRelativePath(KSTR sPath)
	{
		KCHAR drive[_MAX_DRIVE]; 
		KCHAR dir[_MAX_DIR]; 
		KCHAR fname[_MAX_FNAME]; 
		KCHAR ext[_MAX_EXT]; 

		_splitpath( sPath.data(), drive, dir, fname, ext );

		return dir;
	}

	static KSTR GetFileName(KSTR sPath)
	{
		KCHAR drive[_MAX_DRIVE]; 
		KCHAR dir[_MAX_DIR]; 
		KCHAR fname[_MAX_FNAME]; 
		KCHAR ext[_MAX_EXT]; 

		_splitpath( sPath.data(), drive, dir, fname, ext );

		return fname;
	}
	
	static KSTR GetFileNameWithSuffix(KSTR sPath)
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

	static KSTR GetFileSuffix(KSTR sPath)
	{
		KCHAR drive[_MAX_DRIVE]; 
		KCHAR dir[_MAX_DIR]; 
		KCHAR fname[_MAX_FNAME]; 
		KCHAR ext[_MAX_EXT]; 

		_splitpath( sPath.data(), drive, dir, fname, ext );

		return ext;
	}
};