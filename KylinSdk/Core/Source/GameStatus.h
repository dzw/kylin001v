#pragma once

#include "Property.h"

namespace Kylin
{
	enum GStatus
	{
		GS_NONE_,
		GS_LOGIN_,
		GS_LOBBY_,
		GS_GAME_,
	};

	class GameStatus
	{
	public:

		virtual KBOOL Initialize() = 0;	
		virtual KVOID Tick(KFLOAT fElapsed) = 0;
		virtual KVOID Destroy() = 0;
		virtual KVOID Serialize(const PropertySet& kProp){ m_kProperty = kProp; }
		virtual KVOID Deserialize(PropertySet& kProp) { kProp = m_kProperty; }

	protected:
		friend class KylinRoot;
		
		PropertySet m_kProperty;
		GStatus		m_eStatus;
	};

// 	//////////////////////////////////////////////////////////////////////////
// 	class GSLogin : public GameStatus
// 	{
// 	public:
// 		GSLogin(){ m_eStatus = GS_LOGIN_; }
// 		
// 		virtual KBOOL Initialize();
// 		virtual KVOID Tick(KFLOAT fElapsed);
// 		virtual KVOID Destroy();
// 	};
// 	//////////////////////////////////////////////////////////////////////////
// 	class GSLobby : public GameStatus
// 	{
// 	public:
// 		GSLobby(){ m_eStatus = GS_LOBBY_; }
// 
// 	};
	//////////////////////////////////////////////////////////////////////////
	class GSGame : public GameStatus
	{
	public:
		GSGame(){ m_eStatus = GS_GAME_; }

		virtual KBOOL Initialize();
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID Destroy();

	protected:
		friend class KylinRoot;

		WorldManager*	m_pWorldManager;
	};
}