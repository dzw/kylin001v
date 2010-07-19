#pragma once 


namespace Kylin
{
	class Pathway
	{
	public:
		Pathway():m_bIsTurnback(true){}
		
		KPoint3 At(KINT nIndex)
		{
			assert(nIndex >= 0 && nIndex < m_kPoints.size());
			return m_kPoints[nIndex];
		}
		
		KINT Size()
		{
			return m_kPoints.size();
		}
		
		KBOOL IsTurnback() { return m_bIsTurnback; }
		
		KVOID Add(const KPoint3& pos)
		{
			m_kPoints.push_back(pos);
		}
		

		KVOID SetTurnback(KBOOL bFlag)
		{
			m_bIsTurnback = bFlag;
		}

	protected:
		KBOOL			m_bIsTurnback;
		KVEC<KPoint3>	m_kPoints;

	};
}