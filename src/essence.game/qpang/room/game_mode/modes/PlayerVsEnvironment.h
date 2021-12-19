#pragma once

#include "qpang/room/game_mode/GameMode.h"

class PlayerVsEnvironment : public GameMode
{
public:
	bool isMissionMode() override;
	bool isTeamMode() override;
	void onApply(std::shared_ptr<Room> room) override;
};