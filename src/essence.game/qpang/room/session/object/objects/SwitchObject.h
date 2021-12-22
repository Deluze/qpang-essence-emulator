#pragma once

#include "PveObject.h"

class SwitchObject final : public PveObject
{
public:
	SwitchObject(uint32_t linkedGateId, const Position& position);

	void tick(const std::shared_ptr<RoomSession>& roomSession) override;
	void onEvent(const std::shared_ptr<RoomSession>& roomSession) override;
private:
	uint32_t m_linkedGateId;
};