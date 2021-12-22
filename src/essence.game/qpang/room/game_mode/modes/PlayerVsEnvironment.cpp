#include "PlayerVsEnvironment.h"

#include "Room.h"
#include "RoomSession.h"

#include "EscalatorObject.h"
#include "BigGateObject.h"
#include "SmallGateObject.h"
#include "SwitchObject.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_move.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_npc_init.hpp>

bool PlayerVsEnvironment::isMissionMode()
{
	return true;
}

bool PlayerVsEnvironment::isTeamMode()
{
	return true;
}

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
	auto moveInfo = EscalatorMoveInfo { Position { -13.64f, -0.5f, -22.64f }, Position { -1.4f, -0.5f, -22.64f }, 5, 1 };
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	moveInfo = EscalatorMoveInfo{ Position { 14.39f, -0.5f, -23.45f }, Position { 36.56f, -0.5f, -23.45f }, 8, 1 };
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	moveInfo = EscalatorMoveInfo{ Position { -37.7f, -1.2f, -3.4f }, Position { -37.7f, 1.6f, -3.4f }, 4, 0 };
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	moveInfo = EscalatorMoveInfo{ Position { -37.7f, 1.6f, -3.4f + 3.56f }, Position { -37.7f, -1.2f, -3.4f + 3.56f }, 3, 0 };
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	moveInfo = EscalatorMoveInfo{ Position { -37.7f, -1.2f, -3.4f + 3.56f * 2.f }, Position { -37.7f, 1.6f, -3.4f + 3.56f * 2.f }, 3, 0};
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	moveInfo = EscalatorMoveInfo{ Position { -37.7f, 1.6f, -3.4f + 3.56f * 3.f }, Position { -37.7f, -1.2f, -3.4f + 3.56f * 3.f }, 2, 0 };
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	moveInfo = EscalatorMoveInfo{ Position { 23.3f, -1.2f, 13.22f }, Position { 23.3f, 1.6f, 13.22f }, 4, 0 };
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	moveInfo = EscalatorMoveInfo{ Position { 23.3f + 3.3f, 1.6f, 13.22f }, Position { 23.3f + 3.3f, -1.2f, 13.22f }, 3, 0 };
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	moveInfo = EscalatorMoveInfo{ Position { 23.3f + 3.3f * 2.f, -1.2f, 13.22f }, Position { 23.3f + 3.3f * 2.f, 1.6f, 13.22f }, 3, 0 };
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	moveInfo = EscalatorMoveInfo{ Position { 23.3f + 3.3f * 3.f, 1.6f, 13.22f }, Position { 23.3f + 3.3f * 3.f, -1.2f, 13.22f }, 2, 0 };
	roomSession->getObjectManager()->spawnObject(std::make_shared<EscalatorObject>(moveInfo));

	uint32_t bigGateUid = roomSession->getObjectManager()->spawnObject(std::make_shared<BigGateObject>(Position{ 39.1f, 0.f, -5.1f }));
	roomSession->getObjectManager()->spawnObject(std::make_unique<SwitchObject>(bigGateUid, Position{ 25.2f, -6.3f, 37.f }));

	uint32_t smallGateUid = roomSession->getObjectManager()->spawnObject(std::make_shared<SmallGateObject>(Position{ 24.61f, -6.3f, 7.17f }));
	roomSession->getObjectManager()->spawnObject(std::make_unique<SwitchObject>(smallGateUid, Position{ 19.62f, -6.31f, 9.08f }));

	GameMode::onStart(roomSession);
}

void PlayerVsEnvironment::onPlayerSync(const std::shared_ptr<RoomSessionPlayer> session)
{
	// Send all spawned objects/npc/items
	session->getRoomSession()->getObjectManager()->onPlayerSync(session);
	session->getRoomSession()->getNpcManager()->onPlayerSync(session);
	session->getRoomSession()->getPveItemManager()->onPlayerSync(session);

	session->getRoomSession()->relayPlaying<GCPvENpcInit>(eNpcType::EASY_SPY_CAM, 50, Position{ 36, 0, -30 }, (U16)90, (U8)0, (U32)0);

	GameMode::onPlayerSync(session);
}

void PlayerVsEnvironment::tick(const std::shared_ptr<RoomSession> roomSession)
{
	roomSession->getObjectManager()->tick(roomSession);
	roomSession->getNpcManager()->tick(roomSession);
	roomSession->getPveItemManager()->tick(roomSession);

	GameMode::tick(roomSession);
}