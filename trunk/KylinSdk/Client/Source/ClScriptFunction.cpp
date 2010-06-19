
#include "cltpch.h"

#include "ScriptVM.h"
#include "KylinRoot.h"
#include "rOgreRoot.h"
#include "Entity.h"
#include "Character.h"
#include "uiCharInfoMenu.h"
#include "uiShortcutMenu.h"
#include "ActionDispatcher.h"
#include "Action.h"
#include "Avatar.h"
#include "DataManager.h"


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

	extern void to_learn_skill( unsigned int uEntID ,unsigned int uActID)
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Character,pEnt))
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);
			Kylin::Action* pAct		= pChar->GetActionDispatcher()->SpawnAction(uActID);
			
			SAFE_CALL(pAct,SetEmitterNode(pChar->GetSceneNode()));

			Kylin::ShortcutMenu* pMenu = (Kylin::ShortcutMenu*)(Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase("ShortcutMenu"));
			pMenu->SetSkillIcon(pAct->GetIcon(),'l');
		}
	}

	extern void to_bind_weapon( unsigned int uEntID, unsigned int uWeaponID , const char* sNode )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Character,pEnt))
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);
			Kylin::Node* pNode		= pChar->GetAvatar()->AttachWeapon(uWeaponID, strcmp(sNode,"L") ? Kylin::Avatar::AP_RWEAPON : Kylin::Avatar::AP_LWEAPON);

			// 加载武器附加技能
			KANY aRet;
			if (Kylin::DataManager::GetSingletonPtr()->Select("AVATAR_DB",uWeaponID,"SKILL",aRet))
			{
				KINT uActID			= boost::any_cast<KINT>(aRet);	
				Kylin::Action* pAct	= pChar->GetActionDispatcher()->SpawnAction(uActID);
				if (pNode)
				{	SAFE_CALL(pAct,SetEmitterNode(pNode->GetEntityPtr()->getParentNode())); }
				else
				{	SAFE_CALL(pAct,SetEmitterNode(pChar->GetSceneNode())); }
				//---------------------------------------------------------------
				Kylin::ShortcutMenu* pMenu = (Kylin::ShortcutMenu*)(Kylin::OgreRoot::GetSingletonPtr()->GetGuiManager()->GetGuiBase("ShortcutMenu"));
				pMenu->SetSkillIcon(pAct->GetIcon(),'l');
			}
		}
	}

}