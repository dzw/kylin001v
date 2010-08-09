#include "corepch.h"
#include "RemoteEvents.h"

KMAP<KSTR, const EventTemplate*> g_kEventMap;
//////////////////////////////////////////////////////////////////////////
EventTemplate ev_lvl_init				("ev_lvl_init");
EventTemplate ev_lvl_update				("ev_lvl_update");
EventTemplate ev_lvl_timer				("ev_lvl_timer");
EventTemplate ev_lvl_destroy			("ev_lvl_destroy");

//////////////////////////////////////////////////////////////////////////
EventTemplate ev_post_spawn				("ev_post_spawn");
EventTemplate ev_post_attack			("ev_post_attack");
EventTemplate ev_post_damage			("ev_post_damage");
EventTemplate ev_post_picked			("ev_post_picked");
EventTemplate ev_post_pick_terrain		("ev_post_pick_terrain");
EventTemplate ev_post_touchdown			("ev_post_touchdown");
EventTemplate ev_post_throw_item		("ev_post_throw_item");

EventTemplate ev_post_reborn			("ev_post_reborn");
EventTemplate ev_post_killed			("ev_post_killed");
EventTemplate ev_post_destroy			("ev_post_destroy");

//////////////////////////////////////////////////////////////////////////
EventTemplate ev_sync_translate			("ev_sync_translate");
EventTemplate ev_sync_gametime			("ev_sync_gametime");


//////////////////////////////////////////////////////////////////////////
EventTemplate ev_do_spawn				("ev_do_spawn");
EventTemplate ev_on_timer				("ev_on_timer");
EventTemplate ev_do_quit				("ev_do_quit");
EventTemplate ev_sync_gameresult		("ev_sync_gameresult");
EventTemplate ev_restore_hp				("ev_restore_hp");
EventTemplate ev_learn_skill			("ev_learn_skill");
EventTemplate ev_do_teleport			("ev_do_teleport");