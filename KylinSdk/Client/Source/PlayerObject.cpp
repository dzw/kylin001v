#include "cltpch.h"
#include "PlayerObject.h"
#include "ClRegisterClass.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"
#include "uiMiniMapMenu.h"
#include "rOgreRoot.h"
#include "ActionDispatcher.h"
#include "Action.h"
#include "ClScriptFunction.h"
#include "OgreOggSound.h"

#ifdef _DEBUG
#include "ClLobby.h"
#endif

namespace Kylin
{
	BtImplementRTTI(PlayerObject, Character, id_player);

	Implement_Event_Handler(PlayerObject, Character)
	{
		{&ev_post_killed,			&EV_Killed},
		{&ev_post_reborn,			&EV_Reborn},
		{&ev_restore_hp,			&EV_RestoreHP},
		{&ev_learn_skill,			&EV_LearnSkill},
		{&ev_do_teleport,			&Ev_Teleport},
		{NULL, NULL}
	};

	PlayerObject::PlayerObject()
		: m_fRebornTime(10.0f)
	{

	}

	KBOOL PlayerObject::Init( const PropertySet& kProp )
	{
		if ( !Character::Init(kProp) )
			return false;

		return true;
	}
	
	int ttstdaf = 0;
	KVOID PlayerObject::Tick( KFLOAT fElapsed )
	{
		Character::Tick(fElapsed);

// #ifdef _DEBUG
// 		if (ttstdaf++ == 10)
// 		{
// 			KylinRoot::GetSingletonPtr()->SwitchStatus(KNEW ClLobby());
// 			ttstdaf = 0;
// 		}
// #endif
	}

	KVOID PlayerObject::PostSpawn()
	{
		Character::PostSpawn();
		
		// 更新UI
		MiniMapMenu* pMenu = GET_GUI_PTR(MiniMapMenu);
		pMenu->SetMyself(this->GetID());

// 		//-------------------------------
// 		// test code
#ifdef _DEBUG
// 		OgreRoot::GetSingletonPtr()->SetNextPolygonMode();
// 		OgreRoot::GetSingletonPtr()->SetNextPolygonMode();
		
#endif
	}

	KVOID PlayerObject::PostDestroy()
	{
		
		Character::PostDestroy();
	}

	KBOOL PlayerObject::IsMyself()
	{
		return true;
	}

	KVOID PlayerObject::Reborn()
	{
		//
		EventPtr spEV(
			KNEW Event(
			&ev_post_reborn, 
			Event::ev_timing, 
			m_fRebornTime, 
			0, 
			NULL
			));

		KylinRoot::GetSingletonPtr()->PostMessage(this->GetID(),spEV);
	}

	KVOID PlayerObject::EV_Killed( EventPtr spEV )
	{
		KUINT uKiller	= boost::get<unsigned int>(spEV->args[0]);

		Dead();
	}

	KVOID PlayerObject::EV_Reborn( EventPtr spEV )
	{
		this->SetVisible(true);
		
		//-----------------------------------------------------------------
		// 设置出生点
		KANY var;
		if (m_kProperty.GetValue("$BirthPosition",var))
		{
			KPoint3 kPos = boost::any_cast<KPoint3>(var);
			this->SetTranslate(kPos);
		}
		if (m_kProperty.GetValue("$BirthRotation",var))
		{
			KQuaternion kQua = boost::any_cast<KQuaternion>(var);
			this->SetRotation(kQua);
		}
		//-----------------------------------------------------------------
		// 重置生命
		KINT nHp;
		m_kProperty.GetIntValue("$InitHP",nHp);
		m_kProperty.SetValue("$HP",nHp);

		m_eLifeStatus = LS_ALIVE;

		KylinRoot::GetSingletonPtr()->NotifyScriptEntity(this,"on_reborn");
	}

	KVOID PlayerObject::Dead()
	{
		this->SetVisible(false);

		// 重生事件
		Reborn();
	}

	KVOID PlayerObject::EV_RestoreHP( EventPtr spEV )
	{
		KINT nReHp	= boost::get<int>(spEV->args[0]);

		AssertEx(nReHp > 0,"恢复的生命值不允许是负数！");

		KINT nHp,nInitHp;
		m_kProperty.GetIntValue("$InitHP",nInitHp);
		m_kProperty.GetIntValue("$HP",nHp);

		nHp += nReHp;
		if (nHp + nReHp > nInitHp)
			nHp = nInitHp;
		
		m_kProperty.SetValue("$HP",nHp);
	}

	KVOID	PlayerObject::EV_LearnSkill(EventPtr spEV)
	{
		KINT nSkill	= boost::get<int>(spEV->args[0]);
		
		Script::to_learn_skill(this->GetID(),nSkill,true);	
	}

	KVOID PlayerObject::Ev_Teleport( EventPtr spEV )
	{
		KPoint3 pt(0,50000,0);
		pt.x	= boost::get<float>(spEV->args[0]);
		pt.z	= boost::get<float>(spEV->args[1]);
		
		KPoint3 ptRet(pt);
		if ( KylinRoot::GetSingletonPtr()->GetTerrainHeight(ptRet) )//HitTest(pt,KPoint3::NEGATIVE_UNIT_Y,ptRet) )
		{
			pt.y = ptRet.y;
			this->SetTranslate(pt);
		}
	}

	KVOID PlayerObject::SetTranslate( KPoint3 kPos )
	{
		Character::SetTranslate(kPos);

		SAFE_CALL(OgreOggSound::OgreOggSoundManager::getSingletonPtr()->getListener(),setPosition(kPos));
	}
}