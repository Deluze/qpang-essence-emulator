#pragma once

#include "PveNpc.h"

class ViolentRabbitPveNpc : public PveNpc
{
public:
	ViolentRabbitPveNpc(eNpcType type, Position position, uint16_t baseHealth, uint16_t initialSpawnRotation);
};
