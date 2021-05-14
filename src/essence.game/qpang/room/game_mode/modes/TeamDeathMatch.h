#pragma once

#include "qpang/room/game_mode/GameMode.h"

class TeamDeathMatch : public GameMode
{
public:
	bool isTeamMode() override;
};