#pragma once

#include "PveObject.h"

enum class eBigGateActionId {
	STAGE0_TUNNEL_BIG_GATE1
};

class BigGateObject final : public PveObject
{
public:
	BigGateObject(eBigGateActionId actionId, const Position& startPos, const Position& endPos);

	void tick(const std::shared_ptr<RoomSession>& roomSession) override;
private:
	Position m_endPosition;
	eBigGateActionId m_actionId;
};