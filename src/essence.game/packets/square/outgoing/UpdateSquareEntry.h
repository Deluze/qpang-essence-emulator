#pragma once

#include "packets/SquareServerPacket.h"
#include "packets/writers/SquareEntryWriter.h"

#include "qpang/square/Square.h"

class UpdateSquareEntry : public SquareServerPacket
{
public:
	UpdateSquareEntry(const Square::Ptr square, bool isUpdate = true) : SquareServerPacket(6543)
	{
		writeInt(!isUpdate); // 0 = update, 1 = delete

		SquareEntryWriter::write(this, square);
	}
};