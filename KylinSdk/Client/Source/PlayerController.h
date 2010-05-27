#pragma once

#include "InputListener.h"

#define NUM_ANIMS 4           // number of animations the character has
#define CHAR_HEIGHT 5          // height of character's center of mass above ground
#define RUN_SPEED 9//17           // character running speed in units per second
#define TURN_SPEED 1200.0f//500.0f      // character turning in degrees per second
#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
#define JUMP_ACCEL 30.0f       // character jump acceleration in upward units per squared second
#define GRAVITY 90.0f          // gravity in downward units per squared second

namespace Kylin
{
	class GameCamera;
	class Character;
	class PlayerController : public InputListener
	{
	private:
		// all the animations our character has, and a null ID
		// some of these affect separate body parts and will be blended together
		enum AnimID
		{
			ANIM_IDLE_BASE,
			ANIM_WALK_BASE,
			ANIM_RUN_BASE,
			ANIM_JUMP_BASE,
			ANIM_FIGHT_BASE,
			ANIM_NONE
		};

	public:
		PlayerController();
		~PlayerController();

		KVOID			SetTarget(Character* pHost);
		KVOID			SetMoveForwardEnabled(KBOOL bState);

		KBOOL			IsMoveForwardEnabled();
		KBOOL			IsMove();
		
		// 控制跳跃时用的
		KVOID			SetPosY(KFLOAT fPosY);
		KBOOL			GetPosY();
		
		KUINT			GetCurState();
	
	public:
		virtual KVOID	Tick(KFLOAT fElapsed);

	protected:
		KVOID			SetupAnimations();

		KVOID			UpdateBody(KFLOAT fElapsed);
		KVOID			UpdateAnimations(KFLOAT fElapsed);
		
		KVOID			SetBaseAnimation(AnimID id, KBOOL reset = false);
		KVOID			SetTopAnimation(AnimID id, KBOOL reset = false);

	protected:
		virtual KVOID	OnKeyDown(KUINT uKey);
		virtual KVOID	OnKeyUp(KUINT uKey);

		virtual KVOID	OnLButtonDown(KINT nX, KINT nY);
		//virtual KVOID	OnLButtonUp(KINT nX, KINT nY);
		virtual KVOID	OnRButtonDown(KINT nX, KINT nY);
		//virtual KVOID	OnRButtonUp(KINT nX, KINT nY);
		virtual KVOID	OnMouseMove(KFLOAT fX, KFLOAT fY, KFLOAT fZ);

		//virtual KBOOL	OnMidButtonDown();
		
	protected:
		
		Character*	m_pHost;

		KPoint3		m_kKeyDirection;      // player's local intended direction based on WASD keys
		KPoint3		m_kGoalDirection;     // actual intended direction in world-space
		KPoint3		m_kMousePickPos;
		
		KFLOAT		m_fDistance;

		KFLOAT		m_fVerticalVelocity;     // for jumping
		KFLOAT		m_fTimer;                // general timer to see how long animations have been playing
		KFLOAT		m_fCurPosY;
		KBOOL		m_bMoveForward;
		
		AnimationState* m_arAnims[NUM_ANIMS];		// master animation list
		AnimID		m_uBaseAnimID;                  // current base (full- or lower-body) animation
		AnimID		m_uTopAnimID;                   // current top (upper-body) animation
		KBOOL		m_bFadingIn[NUM_ANIMS];         // which animations are fading in
		KBOOL		m_bFadingOut[NUM_ANIMS];        // which animations are fading out

		GameCamera* m_pCamera;
	};
}