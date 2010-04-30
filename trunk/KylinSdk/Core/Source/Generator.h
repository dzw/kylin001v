#pragma once

namespace Kylin
{	
	class Action;
	class Generator
	{
	public:
		Generator();
		virtual ~Generator();

		typedef Action* ActionPtr;
		typedef KVEC<Action*> ActionList;
		
		/** Get the list of profiles that this generator supports.
		*/
		virtual const ActionList& getActions() const { return m_kActions; }
		
		virtual KVOID Generate(ActionPtr spAction)
		{
			if (DoCheck(spAction))
				m_kActions.push_back(spAction);
		}
		
		virtual void Update(KFLOAT fElapsed)
		{
			for (size_t i = 0; i < m_kActions.size(); i++)
			{
				m_kActions[i]->Tick(fElapsed);
			}
		}
	
		virtual KBOOL DoCheck(const ActionPtr spAction)
		{
			
			return true;
		}

	protected:
		ActionList m_kActions;
	};
}