#include "SquareManager.h"

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"

#include "utils/StringConverter.h"

#include "packets/square/outgoing/SendDeleteSquareEntry.h"

SquareManager::SquareManager()
{
	create();
}

Square::Ptr SquareManager::create()
{
	std::u16string prefix = StringConverter::Utf8ToUtf16(CONFIG_MANAGER->getString("SQUARE_PREFIX"));
	uint32_t id = getAvailableSquareId();
	Square::Ptr square = std::make_shared<Square>(id, prefix);

	m_squareMx.lock();
	m_squares.insert_or_assign(id, square);
	m_squareMx.unlock();
	
	return square;
}

Square::Ptr SquareManager::create(const std::u16string& name)
{
	uint32_t id = getAvailableSquareId();
	auto square = std::make_shared<Square>(id, name);

	m_squareMx.lock();
	m_squares.insert_or_assign(id, square);
	m_squareMx.unlock();

	return square;
}

Square::Ptr SquareManager::get(uint32_t id)
{
	std::lock_guard<std::recursive_mutex> lg(m_squareMx);

	auto it = m_squares.find(id);
	return it != m_squares.cend() ? (*it).second : nullptr;
}

Square::Ptr SquareManager::getAvailableSquare()
{
	std::lock_guard<std::recursive_mutex> lg(m_squareMx);

	const auto it = std::find_if(
		m_squares.cbegin(),
		m_squares.cend(),
		[](const std::pair<uint32_t, Square::Ptr>& pair)
		{
			return pair.second->getPlayerCount() < 25;
		}
	);

	return it != m_squares.cend() ? it->second : create();
}

void SquareManager::broadcast(SquareServerPacket packet)
{
	std::lock_guard<std::recursive_mutex> lg(m_squareMx);

	for (const auto& [id, square] : m_squares)
		square->sendPacket(packet);
}

void SquareManager::close(uint32_t id)
{
	if (m_squares.size() == 1)
		return; // we can't remove the last square

	std::lock_guard<std::recursive_mutex> lg(m_squareMx);

	for (const auto& [id, square] : m_squares)
		square->sendPacket(SendDeleteSquareEntry(id));

	m_squares.erase(id);
}

std::vector<Square::Ptr> SquareManager::list()
{
	std::lock_guard<std::recursive_mutex> lg(m_squareMx);

	std::vector<Square::Ptr> squares;

	for (const auto& [id, square] : m_squares)
		squares.push_back(square);

	return squares;
}

uint32_t SquareManager::getAvailableSquareId()
{
	uint32_t id{ 1 };

	std::lock_guard<std::recursive_mutex> lg(m_squareMx);
	
	while (true)
	{
		const auto it = m_squares.find(id);

		if (it == m_squares.cend())
			break;

		id++;
	}

	return id;
}
