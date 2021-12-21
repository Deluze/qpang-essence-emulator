#include "PlayerVsEnvironment.h"

#include "Room.h"
#include "RoomSession.h"

#include "EscalatorObject.h"
#include "BigGateObject.h"
#include "SwitchObject.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_init.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_object_move.hpp>

bool PlayerVsEnvironment::isMissionMode()
{
	return true;
}

bool PlayerVsEnvironment::isTeamMode()
{
	return true;
}

void PlayerVsEnvironment::onApply(std::shared_ptr<Room> room)
{
	room->setIsPointsGame(false);
	room->setScoreTime(10);

	room->setMaxPlayers(4);

	room->setSkillsEnabled(false);

	GameMode::onApply(room);
}

void PlayerVsEnvironment::onStart(std::shared_ptr<RoomSession> roomSession)
{
	roomSession->getObjectManager()->spawnObject(std::make_unique<EscalatorObject>(eEscalatorActionId::STAGE0_PIT1_ESCALATOR1, Position { -13.64f, -0.5f, -22.64f }));
	roomSession->getObjectManager()->spawnObject(std::make_unique<EscalatorObject>(eEscalatorActionId::STAGE0_PIT2_ESCALATOR1, Position { 14.39f, -0.5f, -23.45f }));

	uint32_t gateUid = roomSession->getObjectManager()->spawnObject(std::make_unique<BigGateObject>(eBigGateActionId::STAGE0_TUNNEL_BIG_GATE1, Position{ 39.1f, 0.f, -5.1f }, Position{ 39.1f, 2.f, -5.1f }));
	roomSession->getObjectManager()->spawnObject(std::make_unique<SwitchObject>(gateUid, Position{ 25.2f, -6.3f, 37.f }));

	GameMode::onStart(roomSession);
}

void PlayerVsEnvironment::onPlayerSync(std::shared_ptr<RoomSessionPlayer> session)
{
	// Send all spawned objects/npc/items
	session->getRoomSession()->getObjectManager()->onPlayerSync(session);
	session->getRoomSession()->getNpcManager()->onPlayerSync(session);
	session->getRoomSession()->getPveItemManager()->onPlayerSync(session);

	GameMode::onPlayerSync(session);
}

void PlayerVsEnvironment::tick(std::shared_ptr<RoomSession> roomSession)
{
	roomSession->getObjectManager()->tick(roomSession);
	roomSession->getNpcManager()->tick(roomSession);
	roomSession->getPveItemManager()->tick(roomSession);

	GameMode::tick(roomSession);
}