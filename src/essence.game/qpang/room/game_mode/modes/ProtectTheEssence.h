#pragma once

#include "qpang/room/game_mode/GameMode.h"

class GCRespawn;

class ProtectTheEssence : public GameMode
{
public:
	bool isMissionMode() override;
	bool isTeamMode() override;

	void onApply(std::shared_ptr<Room> room) override;
	void tick(std::shared_ptr<RoomSession> roomSession) override;

	void onPlayerSync(std::shared_ptr<RoomSessionPlayer> sessionPlayer) override;
	void onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation) override;
}; 