#include "cltpch.h"
#include "PlayerObject.h"
#include "ClRegisterClass.h"
#include "RemoteEvents.h"
#include "KylinRoot.h"


namespace Kylin
{
	BtImplementRTTI(PlayerObject, Character, id_player);

	Implement_Event_Handler(PlayerObject, Character)
	{
		{&ev_post_killed,			&EV_Killed},
		{&ev_post_reborn,			&EV_Reborn},
		{&ev_restore_hp,			&EV_RestoreHP},
		{&ev_learn_skill,			&EV_LearnSkill},
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

	KVOID PlayerObject::Tick( KFLOAT fElapsed )
	{
		Character::Tick(fElapsed);

	}

	KVOID PlayerObject::PostSpawn()
	{
		Character::PostSpawn();

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

	}

}