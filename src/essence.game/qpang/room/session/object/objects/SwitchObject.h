#pragma once

#include "PveObject.h"

class SwitchObject : public PveObject
{
public:
	SwitchObject(uint32_t linkedGateId, const Position& position);

	void tick(std::shared_ptr<RoomSession> roomSession) override;
	void onEvent(std::shared_ptr<RoomSession> roomSession) override;
private:
	uint32_t m_linkedGateId;
};