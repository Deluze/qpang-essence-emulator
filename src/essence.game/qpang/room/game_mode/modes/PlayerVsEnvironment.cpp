#include "PlayerVsEnvironment.h"

#include "Room.h"
#include "RoomSession.h"
#include <qpang/room/tnl/net_events/server/gc_pve_npc_init.hpp>

void PlayerVsEnvironment::onApply(const std::shared_ptr<Room> room)
{
	room->setIsPointsGame(false);
	room->setScoreTime(10);

	room->setMaxPlayers(4);

	room->setSkillsEnabled(false);

	GameMode::onApply(room);
}

void PlayerVsEnvironment::onStart(const std::shared_ptr<RoomSession> roomSession)
{
	roomSession->getPveRoundManager()->onStart();
	roomSession->getPveAreaManager()->onStart();
	roomSession->getNpcManager()->onStart();
	roomSession->getPveItemManager()->onStart();
	roomSession->getObjectManager()->onStart();

	GameMode::onStart(roomSession);
}

void PlayerVsEnvironment::onPlayerSync(const std::shared_ptr<RoomSessionPlayer> session)
{
	session->getRoomSession()->getObjectManager()->onPlayerSync(session);
	session->getRoomSession()->getNpcManager()->onPlayerSync(session);
	session->getRoomSession()->getPveItemManager()->onPlayerSync(session);

	//session->send<GCPvENpcInit>(1, 1337, Position{0,0,0}, 0);

	GameMode::onPlayerSync(session);
}

void PlayerVsEnvironment::tick(const std::shared_ptr<RoomSession> roomSession)
{
	roomSession->getObjectManager()->tick();
	roomSession->getNpcManager()->tick();
	roomSession->getPveItemManager()->tick();

	roomSession->getPveRoundManager()->tick();
	roomSession->getPveAreaManager()->tick();

	GameMode::tick(roomSession);
}

void PlayerVsEnvironment::onPlayerKill(
	const std::shared_ptr<RoomSessionPlayer> killer, 
	const std::shared_ptr<RoomSessionPlayer> target, 
	const Weapon& weapon, const uint8_t hitLocation)
{
	// Set can respawn to false, as the player should not suddenly respawn when not selecting a respawn option.
	// This will be overridden in the CGPvERespawnReq packet, when the target makes a choise.
	target->setCanRespawn(false);

	target->resetStreak();
	target->startRespawnCooldown(true);

	// TODO: Add pve statistics.
}

void PlayerVsEnvironment::onPlayerDeathByNpc(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer,
	const std::shared_ptr<PveNpc>& npc)
{
	// Set can respawn to false, as the player should not suddenly respawn when not selecting a respawn option.
	// This will be overridden in the CGPvERespawnReq packet, when the target makes a choise.
	roomSessionPlayer->setCanRespawn(false);

	roomSessionPlayer->resetStreak();
	roomSessionPlayer->startRespawnCooldown(true);

	// TODO: Add pve statistics.

	GameMode::onPlayerDeathByNpc(roomSessionPlayer, npc);
}
