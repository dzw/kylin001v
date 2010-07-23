
#include "cltpch.h"

#include "ScriptVM.h"
#include "KylinRoot.h"
#include "rOgreRoot.h"
#include "Entity.h"
#include "Character.h"
#include "uiCharInfoMenu.h"
#include "uiShortcutMenu.h"
#include "uiMonsterInfoMenu.h"
#include "uiTaskTipsMenu.h"
#include "uiKitbagMenu.h"
#include "uiMiniMapMenu.h"
#include "ActionDispatcher.h"
#include "Action.h"
#include "Avatar.h"
#include "DataManager.h"
#include "ClSceneLoader.h"
#include "Scene.h"
#include "PlayerController.h"
#include "RemoteEvents.h"
#include "ClActionFactory.h"
#include "ClSceneLoader.h"
#include "Pathwayloader.h"
#include "Pathway.h"
#include "NpcObject.h"
#include "Kitbag.h"


namespace Script
{

	extern void ui_reg_char_info( unsigned int uEntID )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Character,pEnt))
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);

			Kylin::CharInfoMenu* pMenu = GET_GUI_PTR(Kylin::CharInfoMenu);

			SAFE_CALL(pMenu,SetRenderMyself(pChar->GetEntityPtr()));
		}
	}

	extern void to_learn_skill( unsigned int uEntID ,unsigned int uActID,bool toUi)
	{
		Kylin::Entity* pEnt			= Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);
		Kylin::ShortcutMenu* pMenu	= GET_GUI_PTR(Kylin::ShortcutMenu);

		if (BtIsKindOf(Kylin::Character,pEnt) && pMenu->HasSpare() > 0)
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);
			Kylin::Action* pAct		= pChar->GetActionDispatcher()->SpawnAction(uActID);
			
			SAFE_CALL(pAct,SetEmitterNode(pChar->GetSceneNode()));
			
			//---------------------------------------------------------------
			if (toUi)
				pMenu->SetSkillInfo(pAct->GetIcon(),pAct->GetCooldawn(),uActID,false,"",pAct->GetExplain());
			//---------------------------------------------------------------
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
				Kylin::ShortcutMenu* pMenu = GET_GUI_PTR(Kylin::ShortcutMenu);
				pMenu->SetSkillInfo(pAct->GetIcon(),pAct->GetCooldawn(),uActID,true,"",pAct->GetExplain());
				//---------------------------------------------------------------
			}
		}
	}

	extern void set_default_action( unsigned int uActID )
	{
		Kylin::ClSceneLoader* pLoader = (Kylin::ClSceneLoader*)Kylin::KylinRoot::GetSingletonPtr()->GetCurrentScene()->GetSceneLoader();
		if (pLoader)
		{
			pLoader->GetController()->SetDefaultAction(uActID);
		}
	}

	extern void set_ui_monster_hp( unsigned int uEntID )
	{
		Kylin::ClSceneLoader* pLoader = (Kylin::ClSceneLoader*)Kylin::KylinRoot::GetSingletonPtr()->GetCurrentScene()->GetSceneLoader();
		if (pLoader)
		{
			KUINT uMonster = pLoader->GetController()->GetSelected();
			if (uMonster == uEntID && uMonster != INVALID_ID)
			{
				Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);
				if (pEnt)
				{
					KINT nHp = 0,nInitHP = 0;
					pEnt->GetPropertyRef().GetIntValue("$HP",nHp);
					pEnt->GetPropertyRef().GetIntValue("$InitHP",nInitHP);

					Kylin::MonsterInfoMenu* pMenu = GET_GUI_PTR(Kylin::MonsterInfoMenu);
					Assert(pMenu);
					pMenu->SetHPWidthPct((float)nHp / (float)nInitHP);
				}
			}
		}
	}

	extern void set_ui_player_hp( unsigned int uEntID )
	{
		Kylin::ClSceneLoader* pLoader = (Kylin::ClSceneLoader*)Kylin::KylinRoot::GetSingletonPtr()->GetCurrentScene()->GetSceneLoader();
		if (pLoader)
		{
			Kylin::Entity* pEnt = pLoader->GetController()->GetHostChar();
			if (pEnt)
			{
				KINT nHp = 0,nInitHP = 0;
				pEnt->GetPropertyRef().GetIntValue("$HP",nHp);
				pEnt->GetPropertyRef().GetIntValue("$InitHP",nInitHP);

				Kylin::ShortcutMenu* pMenu = GET_GUI_PTR(Kylin::ShortcutMenu);
				Assert(pMenu);
				pMenu->SetHPWidthPct((float)nHp / (float)nInitHP);
			}
		}
	}

	extern void post_gameresult( bool bFlag )
	{
		if (bFlag)
		{
			// 游戏胜利，可以进入下一关
			Kylin::TaskTipsMenu* pMenu = GET_GUI_PTR(Kylin::TaskTipsMenu);
			pMenu->SetVisible(true);

		}
		else
		{
			// 游戏失败，十秒后自动退出游戏
			EventPtr spEV(
				KNEW Event(
				&ev_do_quit, 
				Event::ev_timing, 
				10, 
				0, 
				NULL
				));

			Kylin::KylinRoot::GetSingletonPtr()->PostMessage(0,spEV);
		}
	}

	extern void set_default_action_factory( unsigned int uEntID )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);
		if (BtIsKindOf(Kylin::Character,pEnt))
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);

			SAFE_CALL(pChar,SetActionFactory(KNEW Kylin::ClActionFactory(pChar->GetActionDispatcher())));
		}
	}

	extern void set_pathway( unsigned int uEntID, unsigned int uPathwayID )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);
		Kylin::ClSceneLoader* pLoader	= (Kylin::ClSceneLoader*)Kylin::KylinRoot::GetSingletonPtr()->GetCurrentScene()->GetSceneLoader();
		Kylin::Pathway* pPathway		= pLoader->GetPathwayLoader()->GetPathway(uPathwayID);
		assert(pPathway);

		if (BtIsKindOf(Kylin::NpcObject,pEnt))
		{
			Kylin::NpcObject* pNpc = BtStaticCast(Kylin::NpcObject,pEnt);
			SAFE_CALL(pNpc->GetAIHandler(),SetPathway(pPathway));
		}
	}

	extern void add_item( unsigned int uEntID, unsigned int uItemGID )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Character,pEnt))
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);
			pChar->GetKitbag()->AddItem(uItemGID);
			//---------------------------------------------------
			// 更新背包UI
			Kylin::KitbagMenu* pMenu = GET_GUI_PTR(Kylin::KitbagMenu);
			pMenu->Refresh();
		}
	}

	extern void addin_minimap( unsigned int uEntID )
	{
		Kylin::MiniMapMenu* pMenu = GET_GUI_PTR(Kylin::MiniMapMenu);
		Assert(pMenu);
		pMenu->SetBoss(uEntID);
	}
}