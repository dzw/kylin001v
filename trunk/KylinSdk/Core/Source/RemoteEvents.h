#pragma once

#include "Event.h"

// ֪ͨ����Level����Ϣ ǰ׺ ev_lvl_
extern EventTemplate ev_lvl_init;
extern EventTemplate ev_lvl_update;
extern EventTemplate ev_lvl_timer;
extern EventTemplate ev_lvl_destroy;

// ֪ͨentity״̬�ı����Ϣ ǰ׺ ev_post_
extern EventTemplate ev_post_spawn;
extern EventTemplate ev_post_attack;
extern EventTemplate ev_post_damage;
extern EventTemplate ev_post_killed;
extern EventTemplate ev_post_reborn;
extern EventTemplate ev_post_destroy;
extern EventTemplate ev_post_picked;

extern EventTemplate ev_post_pick_terrain;
extern EventTemplate ev_post_touchdown;			// ������Ϣ

extern EventTemplate ev_post_throw_item;		// �������

// ִ��
extern EventTemplate ev_learn_skill;
extern EventTemplate ev_restore_hp;
extern EventTemplate ev_do_spawn;				
extern EventTemplate ev_on_timer;
extern EventTemplate ev_do_quit;				// �˳���Ϸ

// ����ͬ������Ϣ(λ�ã���ת�����ţ�ʱ��) ǰ׺ ev_sync_
extern EventTemplate ev_sync_translate;
extern EventTemplate ev_sync_gametime;
extern EventTemplate ev_sync_gameresult;
//////////////////////////////////////////////////////////////////////////