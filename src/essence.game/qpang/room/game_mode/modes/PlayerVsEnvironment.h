#pragma once

#include "qpang/room/game_mode/GameMode.h"

class PlayerVsEnvironment final : public GameMode
{
public:
	void onApply(std::shared_ptr<Room> room) override;

	void onPlayerSync(std::shared_ptr<RoomSessionPlayer> session) override;
	void onStart(std::shared_ptr<RoomSession> roomSession) override;

	void tick(std::shared_ptr<RoomSession> roomSession) override;
	void onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation) override;
	void onPlayerDeathByNpc(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer, const std::shared_ptr<PveNpc>& npc) override;
};