#pragma once

namespace Kylin
{
	
	class PathwayLoader
	{
	public:
		PathwayLoader();
		~PathwayLoader();

		virtual KBOOL Load(KCCHAR* pScene);

		Pathway* GetPathway(KUINT uID);

	protected:
		KMAP<KUINT,Pathway*>	m_kPathwayMap;
	};

}