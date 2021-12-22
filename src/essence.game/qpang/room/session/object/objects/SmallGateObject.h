#pragma once

#include "PveObject.h"

class SmallGateObject final : public PveObject
{
public:
	SmallGateObject(const Position& position);

	void tick(const std::shared_ptr<RoomSession>& roomSession) override;
private:
};