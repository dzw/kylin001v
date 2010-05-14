#include "corepch.h"
#include "Zone.h"



Kylin::Zone::Zone()
: m_szTiles(NULL)
{
	
}

Kylin::Zone::~Zone()
{
	Destroy();
}

KBOOL Kylin::Zone::Initialize( KCCHAR* pSceneName )
{
	if (m_szTiles) return false;

	KSTR sZone = Ogre::StringUtil::replaceAll(pSceneName,".xml",".zone");

	if (!Open(sZone.data()))
		return false;

	return true;
}

KVOID Kylin::Zone::Destroy()
{
	SAFE_DEL_ARR(m_szTiles);
}

KINT Kylin::Zone::GetTile( KFLOAT x, KFLOAT y )
{
 	KINT nX = m_kHeader.m_nSizeX * KHALF + x / m_kHeader.m_nUnit;
 	KINT nY = m_kHeader.m_nSizeY * KHALF + (y / m_kHeader.m_nUnit) * m_kHeader.m_nSizeX;
 	if (nX < 0 || nY < 0 || nX > m_kHeader.m_nSizeX || nY > m_kHeader.m_nSizeY)
 		return 0;
 
 	return m_szTiles[nX + nY];
}

KVOID Kylin::Zone::SetTile( KFLOAT x, KFLOAT y, KINT val )
{
	KINT nX = m_kHeader.m_nSizeX * KHALF + x / m_kHeader.m_nUnit;
	KINT nY = m_kHeader.m_nSizeY * KHALF + (y / m_kHeader.m_nUnit) * m_kHeader.m_nSizeX;
	if (nX < 0 || nY < 0 || nX > m_kHeader.m_nSizeX || nY > m_kHeader.m_nSizeY)
		return;

	m_szTiles[nX + nY] = val;
}

KBOOL Kylin::Zone::Open( KCCHAR* pName )
{
	FILE* pFile = fopen( pName, "rb" );

	if(NULL == pFile)
		return false;

	fread((void*)&m_kHeader,sizeof(struct ZoneHeader),1,pFile);
	
	if (m_kHeader.m_nSizeX > 0 && m_kHeader.m_nSizeY > 0) 
	{
		m_szTiles = KNEW KINT[m_kHeader.m_nSizeX * m_kHeader.m_nSizeY];
		ZeroMemory(m_szTiles,m_kHeader.m_nSizeX * m_kHeader.m_nSizeY);
		
		fread(m_szTiles,m_kHeader.m_nSizeX*m_kHeader.m_nSizeY,1,pFile);
	}

	fclose(pFile);

	return true;
}

KVOID Kylin::Zone::Save( KCCHAR* pName )
{
	FILE* pFile = fopen( pName, "wb" );
	
	if(NULL == pFile)
		return;

	fwrite((void*)&m_kHeader,sizeof(struct ZoneHeader),1,pFile);
	
	if (m_szTiles && m_kHeader.m_nSizeX > 0 && m_kHeader.m_nSizeY > 0) 
		fwrite(m_szTiles,m_kHeader.m_nSizeX*m_kHeader.m_nSizeY,1,pFile);

	fclose(pFile);
}