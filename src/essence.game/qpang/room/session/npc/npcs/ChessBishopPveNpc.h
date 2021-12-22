#pragma once

#include "PveNpc.h"

class ChessBishopPveNpc : public PveNpc
{
public:
	ChessBishopPveNpc(eNpcType type, Position position, uint16_t baseHealth, uint16_t initialSpawnRotation);
};
