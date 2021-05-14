#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>

#include "qpang/player/friend/Friend.h"

#include "packets/LobbyServerPacket.h"

class Player;

class FriendManager
{
public:
	void initialize(std::shared_ptr<Player> player, uint32_t playerId);

	void appearOnline();
	void appearOffline();

	void send(const LobbyServerPacket& packet);
	void close();

	bool hasIncomingSlotsLeft();
	bool hasOutgoingSlotsLeft();
	bool hasFriendSlotsLeft();
	bool contains(uint32_t playerId);

	void addIncomingFriend(std::shared_ptr<Player> target);
	void addOutgoingFriend(std::shared_ptr<Player> target);

	void removeOutgoing(uint32_t playerId);
	void removeIncoming(uint32_t playerId);

	void acceptIncoming(std::shared_ptr<Player> target);
	void onOutgoingAccepted(std::shared_ptr<Player> target);

	void remove(uint32_t playerId);
	void onRemoved(uint32_t playerId);

	std::vector<Friend> list();
private:
	void setStateDB(uint32_t playerId, uint8_t state);
	void removeFromDB(uint32_t playerId);

	std::mutex m_mx;
	std::weak_ptr<Player> m_player;

	uint16_t m_incomingSlots = 10;
	uint16_t m_outgoingSlots = 10;
	uint16_t m_friendSlots = 30;

	std::unordered_map<uint32_t, Friend> m_friends;
	std::unordered_map<uint32_t, Friend> m_outgoingFriends;
	std::unordered_map<uint32_t, Friend> m_incomingFriends;
};