#pragma once

#include "PveObject.h"

enum class eEscalatorActionId {
	STAGE0_PIT1_ESCALATOR1,
	STAGE0_PIT2_ESCALATOR1
};

class EscalatorObject : public PveObject
{
public:
	EscalatorObject(eEscalatorActionId actionId, const Position& position);

	void tick(std::shared_ptr<RoomSession> roomSession) override;
private:
	eEscalatorActionId m_actionId;
};