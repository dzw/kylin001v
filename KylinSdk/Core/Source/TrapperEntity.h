#pragma once


namespace Kylin
{
	class TrapperEntity : public Entity
	{
	public:
		TrapperEntity();
	
		virtual KVOID Tick(KFLOAT fElapsed);
		
	protected:
		virtual KVOID OnCheck(KFLOAT fElapsed);

	protected:

		KVEC<KUINT>		m_kEntitiesList;
	};
}