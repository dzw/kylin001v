#pragma once

namespace Kylin
{
	struct ZoneHeader
	{
		ZoneHeader()
		{
			m_nSizeX = 0;
			m_nSizeY = 0;
			m_nUnit  = 1;
		}

		KWORD m_nSizeX;
		KWORD m_nSizeY;
		KWORD m_nUnit;
	};

	class Zone
	{
	public:
		Zone();
		virtual ~Zone();
		
		KBOOL Initialize(KCCHAR* pSceneName);
		KVOID Destroy();
		
		KINT  GetTile(KFLOAT x, KFLOAT y);
		KVOID SetTile(KFLOAT x, KFLOAT y, KINT val);

	protected:
		KBOOL Open(KCCHAR* pName);
		KVOID Save(KCCHAR* pName);

	protected:
		ZoneHeader	m_kHeader;
		KINT*		m_szTiles;
	};
}