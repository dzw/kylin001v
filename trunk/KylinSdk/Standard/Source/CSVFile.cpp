#include "stdpch.h"
#include "CSVFile.h"

namespace Kylin
{

CSVFile::CSVFile(KVOID)
{
}

CSVFile::~CSVFile(KVOID)
{

}
// open and load csv file
KBOOL CSVFile::Open(KCCHAR* pFilePath)
{
	ifstream file(pFilePath);
	if(!file)
	{
		return false;
	}
	return this->Init(file);
}
// get the filename of the stream
KCCHAR* CSVFile::GetFilename() const
{
	return filename.c_str();
}
// load csv file and save data to DataTable
KBOOL CSVFile::Init(ifstream &file)
{	
	KSTR sTemp = " ";
	vector<KSTR> vRowData;
	vector<KSTR> vColData;
	m_DataTable.clear();
	m_DataHeaderInfo.clear();
	// get the first row and save to vector
	getline(file,sTemp);
	if (sTemp != "")
	{
		SplitString(sTemp.c_str(), m_DataHeaderInfo);
	}
	// get other rows
	while (!file.eof())
	{
		getline(file,sTemp);
		if (sTemp != "")
		{
			vRowData.push_back(sTemp.c_str());
			sTemp.clear();
		}
	}
	for (KINT i = 0; i < (KINT)vRowData.size(); i++)
	{
		SplitString(vRowData[i].c_str(), vColData);
		// if columns of DataHeader not equal to columns of every row 
		if (vColData.size() != m_DataHeaderInfo.size())
		{
			return false;
		}
		m_DataTable.push_back(vColData);
		vColData.clear();
	}
	return true;
	
}
// split KSTR and save to vector
KINT CSVFile::SplitString(KCCHAR* InString, vector<KSTR>& vColData)
{
	if (InString == NULL)
	{
		return 0;
	}

	KCHAR  *pData = new KCHAR[strlen(InString) + 1];
	memset(pData, 0, strlen(InString) + 1);
	strcpy(pData, InString);

	KCHAR  *pToken = NULL;   
	// split KSTR according to "," or "\t"
	pToken = strtok(pData, ",\t");   

	while( pToken != NULL )   
	{   
		vColData.push_back(pToken);
		pToken = strtok( NULL, ",\t"); 
	}
	return 1;

}
// get max row
KINT CSVFile::GetRowCount(KVOID)
{
	return (KINT)m_DataTable.size();
}
// get max col
KINT CSVFile::GetColCount(KVOID)
{
	if (m_DataHeaderInfo.empty())
	{
		return 0;
	}
	// according to the first line of the table
	return (KINT) m_DataHeaderInfo.size();

}
KBOOL CSVFile::IsRowLineValid(KINT nRow, KINT nCol)
{
	if ((nCol >= GetColCount()) || (nRow >= GetRowCount())||(nCol < 0)||(nRow < 0))
	{
		return false;
	}
	return true;
}
// get KSTR
KCCHAR* CSVFile::GetString(KINT nRow, KINT nCol, KCCHAR* defaultValue)
{
	if (!IsRowLineValid(nRow, nCol))
	{
		return defaultValue;
	}
	return m_DataTable[nRow][nCol].c_str();

}
// get KSTR by Col name
KCCHAR* CSVFile::GetString(KINT nRow, KCCHAR* pColName, KCCHAR* defaultValue)
{
	KINT nCol = FindColIndex(pColName);
	if (!IsRowLineValid(nRow, nCol))
	{
		return defaultValue;
	}
	return m_DataTable[nRow][nCol].c_str();

}
// get KINT
KINT CSVFile::GetInt(KINT nRow, KINT nCol, KINT defaultValue)
{
	if (!IsRowLineValid(nRow, nCol))
	{
		return defaultValue;
	}
	KSTR sData = "";
	sData = m_DataTable[nRow][nCol];
	return	atoi(sData.c_str());
}
// get KINT by Col name
KINT CSVFile::GetInt(KINT nRow, KCCHAR* pColName, KINT defaultValue)
{
	KINT nCol = FindColIndex(pColName);
	if (!IsRowLineValid(nRow, nCol))
	{
		return defaultValue;
	}
	KSTR sData = "";
	sData = m_DataTable[nRow][nCol];
	return	atoi(sData.c_str());

}
// get KFLOAT
KFLOAT CSVFile::GetFloat(KINT nRow, KINT nCol, KFLOAT defaultValue)
{
	if (!IsRowLineValid(nRow, nCol))
	{
		return defaultValue;
	}
	KSTR sData = "";
	sData = m_DataTable[nRow][nCol];
	return	(KFLOAT)atof(sData.c_str());
}
// get KFLOAT by Col name 
KFLOAT CSVFile::GetFloat(KINT nRow, KCCHAR* pColName, KFLOAT defaultValue)
{
	KINT nCol = FindColIndex(pColName);
	if (!IsRowLineValid(nRow, nCol))
	{
		return defaultValue;
	}
	KSTR sData = "";
	sData = m_DataTable[nRow][nCol];
	return	(KFLOAT)atof(sData.c_str());

}

// get KBOOL
KBOOL CSVFile::GetBool(KINT nRow, KINT nCol, KBOOL defaultValue)
{
	if (!IsRowLineValid(nRow, nCol))
	{
		return defaultValue;
	}
	KSTR sData = "";
	sData = m_DataTable[nRow][nCol];
	if (sData == "false")
	{
		return false;
	}
	if (sData == "true")
	{
		return true;
	}
	return	defaultValue;
}
// get KBOOL by Column name
KBOOL CSVFile::GetBool(KINT nRow, KCCHAR* pColName, KBOOL defaultValue)
{
	KINT nCol = FindColIndex(pColName);
	if (!IsRowLineValid(nRow, nCol))
	{
		return defaultValue;
	}
	KSTR sData = "";
	sData = m_DataTable[nRow][nCol];
	if (sData == "false")
	{
		return false;
	}
	if (sData == "true")
	{
		return true;
	}
	return defaultValue;

}
// find col number by col name
KINT CSVFile::FindColIndex(KCCHAR* pColName)
{
	for (KINT i = 0; i < (KINT)m_DataHeaderInfo.size(); i++)
	{
		if (m_DataHeaderInfo[i] == pColName)
		{
			return i;
		}
	}
	return -1;
}
// if has column 
KBOOL CSVFile::HasColumn(KCCHAR* pColName)
{
	for (KINT i = 0; i < (KINT)m_DataHeaderInfo.size(); i++)
	{
		if (m_DataHeaderInfo[i] == pColName)
		{
			return true;
		}
	}
	return false;
}

}