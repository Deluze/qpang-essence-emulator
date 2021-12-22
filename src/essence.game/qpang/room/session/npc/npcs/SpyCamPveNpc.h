#pragma once

#include "PveNpc.h"

class SpyCamPveNpc : public PveNpc
{
public:
	SpyCamPveNpc(eNpcType type, Position position, uint16_t baseHealth, uint16_t initialSpawnRotation);
};
