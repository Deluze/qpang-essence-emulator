#pragma once

#include "PveNpc.h"

class ViolentPlantPveNpc : public PveNpc
{
public:
	ViolentPlantPveNpc(eNpcType type, Position position, uint16_t baseHealth, uint16_t initialSpawnRotation);
};
