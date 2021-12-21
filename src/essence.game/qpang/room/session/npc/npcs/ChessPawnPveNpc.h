#pragma once

#include "PveNpc.h"

class ChessPawnPveNpc : public PveNpc
{
public:
	ChessPawnPveNpc(eNpcType type, Position position, uint16_t baseHealth);
};
