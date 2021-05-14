#include "Vip.h"

#include "qpang/Game.h"
#include "qpang/spawn/SpawnManager.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/room/tnl/net_events/server/gc_respawn.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"

#define VIP_CHANGE_TIME 50

bool Vip::isMissionMode()
{
	return true;
}

bool Vip::isTeamMode()
{
	return true;
}

void Vip::onApply(std::shared_ptr<Room> room)
{
	room->setIsPointsGame(false);
	room->setScoreTime(10);

	GameMode::onApply(room);
}

void Vip::tick(std::shared_ptr<RoomSession> roomSession)
{
	if (roomSession->getElapsedBlueVipTime() > VIP_CHANGE_TIME && !roomSession->isNextBlueVipEligible())
		roomSession->findNextBlueVip();
	if (roomSession->getElapsedYellowVipTime() > VIP_CHANGE_TIME && !roomSession->isNextYellowVipEligible())
		roomSession->findNextYellowVip();
}

void Vip::onPlayerSync(std::shared_ptr<RoomSessionPlayer> sessionPlayer)
{
	auto roomSession = sessionPlayer->getRoomSession();
	auto blueVip = roomSession->getBlueVip();
	auto yellowVip = roomSession->getYellowVip();

	if (blueVip != nullptr)
	{
		auto pos = blueVip->getPosition();
		sessionPlayer->post(new GCRespawn(blueVip->getPlayer()->getId(), blueVip->getCharacter(), 0, pos.x, pos.y, pos.z, true));
		sessionPlayer->post(new GCGameState(blueVip->getPlayer()->getId(), 8));
	}

	if (yellowVip != nullptr)
	{
		auto pos = yellowVip->getPosition();
		sessionPlayer->post(new GCRespawn(yellowVip->getPlayer()->getId(), yellowVip->getCharacter(), 0, pos.x, pos.y, pos.z, true));
		sessionPlayer->post(new GCGameState(yellowVip->getPlayer()->getId(), 8));
	}
}

void Vip::onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation)
{
	auto roomSession = killer->getRoomSession();
	auto blueVip = roomSession->getBlueVip();
	auto yellowVip = roomSession->getYellowVip();

	if (target == blueVip)
	{
		if(killer->getTeam() == 2)
			killer->addScore();
		else
			roomSession->addPointsForTeam(2, 1);
	}
	else if(target == yellowVip)
	{
		if (killer->getTeam() == 1)
			killer->addScore();
		else
			roomSession->addPointsForTeam(1, 1);
	}

	GameMode::onPlayerKill(killer, target, weapon, hitLocation);
}