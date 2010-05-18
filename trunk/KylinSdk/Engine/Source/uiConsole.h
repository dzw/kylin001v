#pragma once

#include "GuiManager.h"
#include "InputListener.h"

namespace Kylin
{
	//////////////////////////////////////////////////////////////////////////
	class DebugConsole : public GuiBase
		, public InputListener
	{
	public:
		DebugConsole();
	public:
		virtual KBOOL Initialize();
		virtual KVOID Render(KFLOAT fElapsed);
		virtual KVOID Destroy();

		virtual KVOID SetVisible(KBOOL bVisible);

		virtual KVOID OnRecept();
		
		virtual KVOID SetHandler(CommandHandler* pHandler);
		
		virtual KVOID PrintLine(KSTR sLine);

	protected:
		void notifyButtonPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

		virtual KVOID OnKeyDown(KUINT uKey);

	private:
		MyGUI::EditPtr			m_spCmdWidget;
		MyGUI::ListPtr			m_spContentWidget;
		
		CommandHandler*			m_pCmdHandler;
		//////////////////////////////////////////////////////////////////////////
		KVEC<KSTR>				m_kStoredList;
		KINT					m_nCurrIndex;
	};
}