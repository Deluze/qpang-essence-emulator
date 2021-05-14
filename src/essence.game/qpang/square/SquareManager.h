#pragma once

#include <map>
#include <mutex>
#include <vector>

#include "qpang/square/Square.h"
#include "packets/SquareServerPacket.h"

class SquareManager
{
public:
	SquareManager();

	Square::Ptr create();
	Square::Ptr create(const std::u16string& name);
	Square::Ptr get(uint32_t id);
	Square::Ptr getAvailableSquare();
	void broadcast(SquareServerPacket packet);
	void close(uint32_t id);

	std::vector<Square::Ptr> list();
private:
	uint32_t getAvailableSquareId();
	std::map<uint32_t, Square::Ptr> m_squares;

	std::recursive_mutex m_squareMx;
};