#pragma once

namespace Kylin
{

	class CSVFile
	{
	public:

		CSVFile(void);
		~CSVFile(void);
		// open and load csv file
		KBOOL Open(KCCHAR* pFilePath);
		// get max row
		KINT GetRowCount(KVOID);
		// get max col
		KINT GetColCount(KVOID);
		// get KSTR
		KCCHAR* GetString(KINT nRow, KINT nCol, KCCHAR* defaultValue = "");
		// get KSTR by Col name
		KCCHAR* GetString(KINT nRow, KCCHAR* pColName, KCCHAR* defaultValue = "");
		// get KBOOL
		KBOOL GetBool(KINT nRow, KINT nCol, KBOOL defaultValue = false);
		// get KBOOL by Col name
		KBOOL GetBool(KINT nRow, KCCHAR* pColName, KBOOL defaultValue = false);
		// get KINT
		KINT GetInt(KINT nRow, KINT nCol, KINT defaultValue = 0);
		// get KINT by Col name
		KINT GetInt(KINT nRow, KCCHAR* pColName, KINT defaultValue = 0);
		// get KFLOAT
		KFLOAT GetFloat(KINT nRow, KINT nCol, KFLOAT defaultValue = 0.0);
		// get KFLOAT by Col name
		KFLOAT GetFloat(KINT nRow, KCCHAR* pColName, KFLOAT defaultValue = 0.0);
		// get the filename of the stream
		KCCHAR* GetFilename() const;
		// if has column 
		KBOOL HasColumn(KCCHAR* pColName);
		// find col number by col name
		KINT FindColIndex(KCCHAR* pColName);

	private:
		// load csv file and save data to DataTable
		KBOOL Init(ifstream &file);
		// split KSTR and save to vector
		KINT SplitString(KCCHAR* sTemp,	vector<KSTR>& vLineData);
		// judge row && line valid or not 
		KBOOL IsRowLineValid(KINT nCol, KINT nRow);
		KSTR filename;
		// DataTable[i][j] indicate the data in row i and column j
		typedef std::vector< std::vector<KSTR> > DATATABLE;
		vector<KSTR> m_DataHeaderInfo;	// the first line of the table
		DATATABLE m_DataTable;				// data table

	};

}