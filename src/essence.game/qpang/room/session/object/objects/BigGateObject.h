#pragma once

#include "PveObject.h"

class BigGateObject final : public PveObject
{
public:
	BigGateObject(const Position& startPos);

	void tick(const std::shared_ptr<RoomSession>& roomSession) override;
private:
};