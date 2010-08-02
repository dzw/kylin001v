#pragma once

#include "rRenderable.h"

namespace Kylin
{
	class RenderableManager
	{
	public:

		~RenderableManager()
		{
			m_kRenderableList.clear();
		}

	public:
		virtual KVOID OnRenderStarted(KFLOAT fElapsed)
		{
			for (KUINT i = 0; i < m_kRenderableList.size(); i++)
			{
				if (m_kRenderableList[i])
					m_kRenderableList[i]->OnRenderStarted(fElapsed);
			}
		}

		virtual KVOID OnRenderEnded(KFLOAT fElapsed)
		{
			for (KUINT i = 0; i < m_kRenderableList.size(); i++)
			{
				if (m_kRenderableList[i])
					m_kRenderableList[i]->OnRenderEnded(fElapsed);
			}
		}

		KVOID Add(Renderable* pObj)
		{
			for (KUINT i = 0; i < m_kRenderableList.size(); i++)
			{
				if (m_kRenderableList[i] == pObj)
					return;
			}

			m_kRenderableList.push_back(pObj);
		}

		KVOID Remove(Renderable* pObj)
		{
			for (KUINT i = 0; i < m_kRenderableList.size(); i++)
			{
				if (m_kRenderableList[i] == pObj)
				{
					m_kRenderableList.erase(m_kRenderableList.begin()+i);
					return;
				}
			}
		}

	protected:
		KVEC<Renderable*>	m_kRenderableList;
	};
}