#include "corepch.h"

#include ".\scriptfunctions.h"
#include "ScriptVM.h"
#include "rOgreRoot.h"
#include "KylinRoot.h"
#include "Entity.h"
#include "AnimationProxy.h"
#include "Character.h"
#include "Avatar.h"


namespace Script
{

	void lua_break(const char * sDebugInfo)
	{
		OutputDebugString(sDebugInfo);
		DebugBreak();
	}
	

	extern void set_next_anim( unsigned int uEntID, const char* sAnim, float fTimes )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (pEnt)
		{
			pEnt->GetAnimationProxy()->Play(sAnim,fTimes < 0);
		}
	}

	extern void set_effect( unsigned int uEntID, unsigned int uEffectID)
	{
		int i = 0;
	}

	extern void set_global_effect( unsigned int uEffectID )
	{
		int i = 0;
	}

	extern void bind_weapon( unsigned int uEntID, unsigned int uWeaponID , const char* sNode )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Character,pEnt))
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);
			pChar->GetAvatar()->AttachWeapon(uWeaponID, strcmp(sNode,"L") ? Kylin::Avatar::AP_RWEAPON : Kylin::Avatar::AP_LWEAPON);
		}
	}

	extern void exchange_avatar( unsigned int uEntID, unsigned int uAvatarID )
	{
		Kylin::Entity* pEnt = Kylin::KylinRoot::GetSingletonPtr()->GetEntity(uEntID);

		if (BtIsKindOf(Kylin::Character,pEnt))
		{
			Kylin::Character* pChar = BtStaticCast(Kylin::Character,pEnt);
			pChar->GetAvatar()->Exchange(uAvatarID);
		}
	}

	//-----------------------------------------------------
	extern void set_fog_color( float r, float g, float b )
	{
		
	}

	extern void set_fog_distance( float d )
	{

	}

	extern void set_fog_intensity( float i )
	{

	}

	extern void set_fog_near( float n )
	{

	}

	extern void set_fog_far( float f )
	{

	}

	extern void set_fog_enable( bool b )
	{
		
	}

	extern void set_camera_near( float n )
	{

	}

	extern void set_camera_far( float f )
	{

	}

	extern void set_visible_distance( float d )
	{

	}

	extern void set_viewport_back( float r, float g, float b )
	{

	}
}