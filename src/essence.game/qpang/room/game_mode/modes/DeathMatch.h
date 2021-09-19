#pragma once

#include "qpang/room/game_mode/GameMode.h"

class DeathMatch : public GameMode
{
public:
	virtual void onApply(std::shared_ptr<Room> room) override;
};