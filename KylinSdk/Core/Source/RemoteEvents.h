#pragma once

#include "Event.h"

// 通知场景Level的消息 前缀 ev_lvl_
extern EventTemplate ev_lvl_init;
extern EventTemplate ev_lvl_update;
extern EventTemplate ev_lvl_timer;
extern EventTemplate ev_lvl_destroy;

// 通知entity状态改变的消息 前缀 ev_post_
extern EventTemplate ev_post_spawn;
extern EventTemplate ev_post_attack;
extern EventTemplate ev_post_damage;
extern EventTemplate ev_post_killed;
extern EventTemplate ev_post_destroy;
extern EventTemplate ev_post_picked;

extern EventTemplate ev_post_pick_terrain;

// 用于同步的消息(位置，旋转，缩放，时间) 前缀 ev_sync_
extern EventTemplate ev_sync_translate;
extern EventTemplate ev_sync_gametime;

//////////////////////////////////////////////////////////////////////////