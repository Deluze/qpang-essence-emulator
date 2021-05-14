#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include <mutex>

#include "qpang/square/SquarePlayer.h"
#include "packets/SquareServerPacket.h"

class Player;

class Square : public std::enable_shared_from_this<Square>
{
public:
	using Ptr = std::shared_ptr<Square>;

	Square(uint32_t id, std::u16string name, uint8_t capacity = 50);

	bool add(std::shared_ptr<Player> player);

	void remove(uint32_t player);

	void sendPacket(const SquareServerPacket& packet);
	void sendPacketExcept(const SquareServerPacket& packet, uint32_t playerId);

	void setClosed(bool isClosed);

	std::vector<SquarePlayer::Ptr> listPlayers();
	SquarePlayer::Ptr getSquarePlayer(uint32_t playerId);

	uint32_t getId();
	std::u16string getName();
	uint8_t getCapacity();
	uint8_t getPlayerCount();
	uint8_t getState();
	bool isFull();
	bool isClosed();
private:

	std::unordered_map<uint32_t, SquarePlayer::Ptr> m_players;
	std::recursive_mutex m_playerMx;

	const uint32_t m_id;
	std::u16string m_name;
	uint8_t m_capacity;
	uint8_t m_state;
	bool m_isClosed;
};