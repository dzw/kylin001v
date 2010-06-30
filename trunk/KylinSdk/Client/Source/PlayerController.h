#pragma once

#include "InputListener.h"

#define NUM_ANIMS 4           // number of animations the character has
#define CHAR_HEIGHT 5          // height of character's center of mass above ground
#define RUN_SPEED 9//17           // character running speed in units per second
#define TURN_SPEED 1200.0f//500.0f      // character turning in degrees per second
#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
#define JUMP_ACCEL 50.0f       // character jump acceleration in upward units per squared second
#define GRAVITY 90.0f          // gravity in downward units per squared second


namespace Kylin
{

	struct UsedAction
	{
		UsedAction()
		{
			Reset();
		}
		
		KVOID Reset()
		{
			uActionGID = INVALID_ID;
			uActionType= INVALID_ID;
		}

		KUINT uActionGID;
		KUINT uActionType;
	};

	class GameCamera;
	class Character;
	class PlayerController : public InputListener
	{
	public:
		PlayerController();
		~PlayerController();

		KVOID			SetTarget(Character* pHost);
		KVOID			SetMoveForwardEnabled(KBOOL bState);

		KBOOL			IsMoveForwardEnabled();
		KBOOL			IsMove();
		
		// test
		KVOID			UseSkill(KUINT uActID);

		// ���ý���
		KVOID			FocusTarget(KUINT uTargetID);

	public:
		virtual KVOID	Tick(KFLOAT fElapsed);

	protected:
		KVOID			UpdateBody(KFLOAT fElapsed);

		// ���½�����Ч
		KVOID			UpdateEffect(KFLOAT fElapsed);
		
		KBOOL			SelectedEntity(Ogre::Ray kRay);
		KVOID			SelectedTerrain(Ogre::Ray kRay);

	protected:
		virtual KVOID	OnKeyDown(KUINT uKey);
		virtual KVOID	OnKeyUp(KUINT uKey);

		virtual KVOID	OnLButtonDown(KINT nX, KINT nY);
		virtual KVOID	OnRButtonDown(KINT nX, KINT nY);
		virtual KVOID	OnMouseMove(KFLOAT fX, KFLOAT fY, KFLOAT fZ);
		virtual KVOID	OnMouseMove(KINT nX, KINT nY);

	protected:
		
		Character*		m_pHost;

		KPoint3			m_kKeyDirection;      // player's local intended direction based on WASD keys
		KPoint3			m_kGoalDirection;     // actual intended direction in world-space
		KPoint3			m_kMousePickPos;
		
		KFLOAT			m_fDistance;
		KBOOL			m_bMoveForward;
		KBOOL			m_bPrepareFire;

		// ������Ч
		EffectDecal*	m_pGuideEffect;
		// ѡ�ж�����Ч
		EffectDecal*	m_pFocusEffect;
		// ��������
		KUINT			m_uTargetID;
		// ��ǰ��ѡ��ļ���
		UsedAction		m_kSelectAction;

		// �����
		GameCamera*		m_pCamera;
	};
}