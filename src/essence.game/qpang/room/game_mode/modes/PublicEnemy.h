#pragma once

#include "qpang/room/game_mode/GameMode.h"
#include "qpang/player/Player.h"
#include "qpang/room/session/RoomSession.h"

class PublicEnemy : public GameMode
{
public:
	bool isMissionMode() override;
	bool isTeamMode() override;
	bool isPublicEnemyMode() override;

	void onApply(std::shared_ptr<Room> room) override;
	void tick(std::shared_ptr<RoomSession> roomSession) override;

	void onPlayerSync(std::shared_ptr<RoomSessionPlayer> sessionPlayer) override;
	void onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation) override;
	void broadcastTagKill(RoomSession::Ptr roomSession, Player::Ptr killer, bool isSuicide);
};