#pragma once

#include <vector>

#include "packets/SquareServerPacket.h"
#include "packets/writers/SquareEntryWriter.h"
#include "qpang/square/Square.h"

class SquareList : public SquareServerPacket
{
public:
	explicit SquareList(const std::vector<Square::Ptr>& squares) : SquareServerPacket(6501)
	{
		const uint16_t size = static_cast<uint16_t>(squares.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		for (const Square::Ptr& square : squares)
		{
			SquareEntryWriter::write(this, square);
		}
	}
};
