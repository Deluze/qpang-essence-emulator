#pragma once

#include "PveObject.h"

enum class eEscalatorActionId {
	STAGE0_PIT1_ESCALATOR1,
	STAGE0_PIT2_ESCALATOR1
};

class EscalatorObject final : public PveObject
{
public:
	EscalatorObject(eEscalatorActionId actionId, const Position& position);

	void tick(const std::shared_ptr<RoomSession>& roomSession) override;
private:
	eEscalatorActionId m_actionId;
};