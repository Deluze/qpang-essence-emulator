#include "Square.h"

#include <iostream>

#include "qpang/Game.h"
#include "qpang/square/SquareManager.h"
#include "qpang/square/SquarePlayer.h"

#include "packets/square/outgoing/JoinSquareSuccess.h"
#include "packets/square/outgoing/AddPlayer.h"
#include "packets/square/outgoing/RemovePlayer.h"
#include "packets/square/outgoing/UpdateSquareEntry.h"

Square::Square(uint32_t id, std::u16string name, uint8_t capacity) :
	m_id(id),
	m_name(std::move(name)),
	m_capacity(capacity),
	m_state(8),
	m_isClosed(false)
{

}

bool Square::add(std::shared_ptr<Player> player)
{
	assert(player != nullptr);

	if (getPlayerCount() >= m_capacity)
		return false;

	if (player->getRank() == 1 && m_isClosed)
		return false;

	const auto curr = shared_from_this();
	
	const auto squarePlayer = std::make_shared<SquarePlayer>(player, curr);

	m_playerMx.lock();
	m_players[player->getId()] = squarePlayer;
	m_playerMx.unlock();

	Game::instance()->getSquareManager()->broadcast(UpdateSquareEntry(curr, true));

	player->enterSquare(squarePlayer);

	sendPacketExcept(AddPlayer(squarePlayer), player->getId());

	return true;
}

void Square::remove(uint32_t playerId)
{
	m_playerMx.lock();
	m_players.erase(playerId);
	m_playerMx.unlock();

	sendPacket(RemovePlayer(playerId));
	
	const auto curr = shared_from_this();
	Game::instance()->getSquareManager()->broadcast(UpdateSquareEntry(curr, true));

	if (m_players.empty())
		Game::instance()->getSquareManager()->close(m_id);
}

void Square::sendPacket(const SquareServerPacket& packet)
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	for (const auto& [id, sqPlayer] : m_players)
		sqPlayer->getPlayer()->send(packet);
}

void Square::sendPacketExcept(const SquareServerPacket& packet, uint32_t playerId)
{
	std::lock_guard<std::recursive_mutex>  lg(m_playerMx);

	for (const auto& [id, sqPlayer] : m_players)
		if (sqPlayer->getPlayer()->getId() != playerId)
			sqPlayer->getPlayer()->send(packet);
}

void Square::setClosed(bool isClosed)
{
	m_isClosed = isClosed;
}

std::vector<SquarePlayer::Ptr> Square::listPlayers()
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	std::vector<SquarePlayer::Ptr> players;

	for (const auto& [id, player] : m_players)
		players.push_back(player);

	return players;
}

SquarePlayer::Ptr Square::getSquarePlayer(uint32_t playerId)
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	auto it = m_players.find(playerId);
	return it != m_players.cend() ? (*it).second : nullptr;
}

uint32_t Square::getId()
{
	return m_id;
}

std::u16string Square::getName()
{
	return m_name;
}

uint8_t Square::getCapacity()
{
	return m_capacity;
}

uint8_t Square::getPlayerCount()
{
	return m_players.size();
}

uint8_t Square::getState()
{
	return m_state;
}

bool Square::isFull()
{
	return m_players.size() >= m_capacity;
}

bool Square::isClosed()
{
	return m_isClosed;
}
