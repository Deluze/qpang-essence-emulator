#include "cc_charm.hpp"
#include "cc_user_info.hpp"
#include "cg_arranged_complete.hpp"
#include "cg_arranged_reject.hpp"
#include "cg_auth.hpp"
#include "cg_card.hpp"
#include "cg_charm.hpp"
#include "cg_essence.hpp"
#include "cg_exit.hpp"
#include "cg_game_item.hpp"
#include "cg_game_state.hpp"
#include "cg_hit.hpp"
#include "cg_hit_essence.hpp"
#include "cg_log.hpp"
#include "cg_map_object.hpp"
#include "cg_mesg.hpp"
#include "cg_motion.hpp"
#include "cg_move.hpp"
#include "cg_move_report.hpp"
#include "cg_player_change.hpp"
#include "cg_pve_area_trigger.hpp"
#include "cg_pve_event_object.hpp"
#include "cg_pve_floor.hpp"
#include "cg_pve_get_item.hpp"
#include "cg_pve_hit_n2p.hpp"
#include "cg_pve_hit_npc.hpp"
#include "cg_pve_hit_npc_to_object.hpp"
#include "cg_pve_hit_object.hpp"
#include "cg_pve_respawn_req.hpp"
#include "cg_pve_round_start.hpp"
#include "cg_pve_start.hpp"
#include "cg_ready.hpp"
#include "cg_respawn.hpp"
#include "cg_room.hpp"
#include "cg_room_info.hpp"
#include "cg_score.hpp"
#include "cg_shoot.hpp"
#include "cg_shoot_report.hpp"
#include "cg_start.hpp"
#include "cg_sync.hpp"
#include "cg_target.hpp"
#include "cg_weapon.hpp"
#include "cs_rtt_request.hpp"
#include "gc_arranged_accept.hpp"
#include "gc_arranged_conn.hpp"
#include "gc_card.hpp"
#include "gc_charm.hpp"
#include "gc_disconnect.hpp"
#include "gc_essence.hpp"
#include "gc_exit.hpp"
#include "gc_game_item.hpp"
#include "gc_game_state.hpp"
#include "gc_hit.hpp"
#include "gc_hit_essence.hpp"
#include "gc_join.hpp"
#include "gc_map_object.hpp"
#include "gc_master_log.hpp"
#include "gc_mesg.hpp"
#include "gc_motion.hpp"
#include "gc_move.hpp"
#include "gc_player_change.hpp"
#include "gc_pve_area_trigger_init.hpp"
#include "gc_pve_destroy_object.hpp"
#include "gc_pve_destroy_parts.hpp"
#include "gc_pve_die_npc.hpp"
#include "gc_pve_door.hpp"
#include "gc_pve_event_object.hpp"
#include "gc_pve_hit_n2n.hpp"
#include "gc_pve_hit_n2p.hpp"
#include "gc_pve_hit_npc.hpp"
#include "gc_pve_hit_npc_to_object.hpp"
#include "gc_pve_hit_object.hpp"
#include "gc_pve_item_init.hpp"
#include "gc_pve_move_nodes_npc.hpp"
#include "gc_pve_move_npc.hpp"
#include "gc_pve_new_round.hpp"
#include "gc_pve_npc_init.hpp"
#include "gc_pve_object_init.hpp"
#include "gc_pve_object_move.hpp"
#include "gc_pve_round_end.hpp"
#include "gc_pve_score_result.hpp"
#include "gc_pve_shoot_n2p.hpp"
#include "gc_pve_special_attack.hpp"
#include "gc_pve_start.hpp"
#include "gc_pve_user_init.hpp"
#include "gc_quest.hpp"
#include "gc_ready.hpp"
#include "gc_respawn.hpp"
#include "gc_room.hpp"
#include "gc_room_info.hpp"
#include "gc_score.hpp"
#include "gc_score_result.hpp"
#include "gc_shoot.hpp"
#include "gc_start.hpp"
#include "gc_sync.hpp"
#include "gc_target.hpp"
#include "gc_weapon.hpp"
#include "gg_reload.hpp"
#include "p_cs_rtt_report.hpp"
#include "p_cs_rtt_response.hpp"

TNL_IMPLEMENT_NETEVENT(CCCharm, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CCUserInfo, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGArrangedComplete, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGArrangedReject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGAuth, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGCard, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGCharm, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGEssence, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGExit, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGGameItem, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGGameState, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGHit, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGHitEssence, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGLog, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGMapObject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGMesg, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGMotion, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGMove, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGMoveReport, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPlayerChange, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvEAreaTrigger, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvEEventObject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvEFloor, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvEGetItem, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvEHitN2P, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvEHitNpc, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvEHitNpcToObject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvEHitObject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvERespawnReq, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvERoundStart, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGPvEStart, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGReady, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGRespawn, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGRoom, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGRoomInfo, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGScore, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGShoot, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGShootReport, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGStart, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGSync, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGTarget, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CGWeapon, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(CSRttRequest, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCArrangedAccept, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCArrangedConn, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCCard, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCCharm, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCDisconnect, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCEssence, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCExit, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCGameItem, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCGameState, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCHit, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCHitEssence, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCJoin, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCMapObject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCMasterLog, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCMesg, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCMotion, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCMove, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPlayerChange, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEAreaTriggerInit, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEDestroyObject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEDestroyParts, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEDieNpc, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEDoor, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEEventObject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEHitN2N, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEHitN2P, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEHitNpc, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEHitNpcToObject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEHitObject, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEItemInit, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEMoveNodesNpc, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEMoveNpc, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvENewRound, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvENpcInit, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEObjectInit, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEObjectMove, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvERoundEnd, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEScoreResult, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEShootN2P, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvESpecialAttack, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEStart, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCPvEUserInit, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCQuest, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCReady, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCRespawn, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCRoom, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCRoomInfo, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCScore, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCScoreResult, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCShoot, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCStart, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCSync, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCTarget, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GCWeapon, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(GGReload, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(P_CSRttReport, TNL::NetClassGroupGameMask, 0);
TNL_IMPLEMENT_NETEVENT(P_CSRttResponse, TNL::NetClassGroupGameMask, 0);
