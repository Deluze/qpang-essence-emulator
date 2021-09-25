#pragma once

#include "qpang/room/game_mode/GameMode.h"

class Practice : public GameMode
{
public:
	bool isMissionMode() override;
	bool isTeamMode() override;
};