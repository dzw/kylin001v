
#include "cltpch.h"

#include "ScriptVM.h"
#include "KylinRoot.h"
#include "rOgreRoot.h"
#include "Entity.h"
#include "Character.h"
#include "uiCharInfoMenu.h"


namespace Script
{

	extern void ui_reg_char_info( unsigned int uEntID )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Character,pEnt))
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);

			Kylin::CharInfoMenu* pMenu = (Kylin::CharInfoMenu*)(Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase("CharInfoMenu"));
			pMenu->SetRenderMyself(pChar->GetEntityPtr());
		}
	}

}