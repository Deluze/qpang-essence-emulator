#include "FriendManager.h"

#include "core/Emulator.h"
#include "core/database/Database.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"

#include "packets/lobby/outgoing/friend/SendAppearOnline.h"
#include "packets/lobby/outgoing/friend/SendAppearOffline.h"
#include "packets/lobby/outgoing/friend/SendAddIncomingFriend.h"
#include "packets/lobby/outgoing/friend/SendAddOutgoingFriend.h"
#include "packets/lobby/outgoing/friend/SendOutgoingFriendAccepted.h"
#include "packets/lobby/outgoing/friend/SendAcceptIncomingFriend.h"
#include "packets/lobby/outgoing/friend/SendRemoveFriend.h"
#include "packets/lobby/outgoing/friend/SendFriendRemoved.h"

void FriendManager::initialize(std::shared_ptr<Player> player, uint32_t playerId)
{
	m_player = player;

	auto stmt = DATABASE->prepare("SELECT * FROM friends JOIN players ON players.id = friends.player_to WHERE friends.player_from = ?");
	stmt->bindInteger(playerId);
	auto res = stmt->fetch();

	std::lock_guard<std::mutex> lg(m_mx);

	while (res->hasNext())
	{
		auto friendId = res->getInt("player_to");

		Friend fr{
			friendId,
			res->getString("name"),
			res->getTiny("level"),
			res->getTiny("rank"),
			res->getTiny("status"),
			fr.state == 1 ? Game::instance()->getOnlinePlayer(friendId) != nullptr : false,
		};

		switch (fr.state)
		{
		case 1:
			m_friends[friendId] = fr;
			break;
		case 2:
			m_outgoingFriends[friendId] = fr;
			break;
		case 4:
		default:
			m_incomingFriends[friendId] = fr;
		}

		res->next();
	}
}

void FriendManager::appearOnline()
{
	if (const auto player = m_player.lock(); player != nullptr)
		send(SendAppearOnline(player->getId()));
}

void FriendManager::appearOffline()
{
	if (const auto player = m_player.lock(); player != nullptr)
		send(SendAppearOffline(player->getId()));
}

void FriendManager::send(const LobbyServerPacket& packet)
{
	std::lock_guard<std::mutex> lg(m_mx);

	for (const auto& [playerId, fr] : m_friends)
		if (const auto player = Game::instance()->getOnlinePlayer(playerId); player != nullptr)
			player->send(packet);
}

bool FriendManager::hasIncomingSlotsLeft()
{
	return m_incomingFriends.size() < m_incomingSlots;
}

bool FriendManager::hasOutgoingSlotsLeft()
{
	return m_outgoingFriends.size() < m_outgoingSlots;
}

bool FriendManager::hasFriendSlotsLeft()
{
	return m_friends.size() < m_friendSlots;
}

bool FriendManager::contains(uint32_t playerId)
{
	std::lock_guard<std::mutex> lg(m_mx);

	if (auto it = m_friends.find(playerId); it != m_friends.cend())
		return true;

	if (auto it = m_incomingFriends.find(playerId); it != m_incomingFriends.cend())
		return true;

	if (auto it = m_outgoingFriends.find(playerId); it != m_outgoingFriends.cend())
		return true;

	return false;
}

void FriendManager::addIncomingFriend(std::shared_ptr<Player> target)
{
	assert(target != nullptr);

	if (auto player = m_player.lock(); player != nullptr)
	{
		DATABASE_DISPATCHER->dispatch("INSERT INTO friends (player_from, player_to, status) VALUES (?, ?, 4)", { player->getId(), target->getId() });

		Friend fr{
			target->getId(),
			target->getName(),
			target->getLevel(),
			target->getRank(),
			4,
			false,
		};

		std::lock_guard<std::mutex> lg(m_mx);

		m_incomingFriends[fr.playerId] = fr;

		player->send(SendAddIncomingFriend(fr));
	}
}

void FriendManager::addOutgoingFriend(std::shared_ptr<Player> target)
{
	assert(target != nullptr);

	if (auto player = m_player.lock(); player != nullptr)
	{
		DATABASE_DISPATCHER->dispatch("INSERT INTO friends (player_from, player_to, status) VALUES (?, ?, 2)", { player->getId(), target->getId() });

		Friend fr{
			target->getId(),
			target->getName(),
			target->getLevel(),
			target->getRank(),
			2,
			false,
		};

		std::lock_guard<std::mutex> lg(m_mx);

		m_outgoingFriends[fr.playerId] = fr;

		player->send(SendAddOutgoingFriend(fr));
	}
}

void FriendManager::removeOutgoing(uint32_t playerId)
{
	std::lock_guard<std::mutex> lg(m_mx);

	auto it = m_outgoingFriends.find(playerId);

	if (it == m_outgoingFriends.cend())
		return;

	m_outgoingFriends.erase(it);

	removeFromDB(playerId);
}

void FriendManager::removeIncoming(uint32_t playerId)
{
	std::lock_guard<std::mutex> lg(m_mx);

	auto it = m_incomingFriends.find(playerId);

	if (it == m_incomingFriends.cend())
		return;

	m_incomingFriends.erase(it);

	removeFromDB(playerId);
}

void FriendManager::acceptIncoming(std::shared_ptr<Player> target)
{
	if (auto player = m_player.lock(); player != nullptr)
	{
		std::lock_guard<std::mutex> lg(m_mx);

		auto it = m_incomingFriends.find(target->getId());

		if (it == m_incomingFriends.cend())
			return;

		Friend fr = (*it).second;
		fr.state = 1;
		fr.isOnline = target->isOnline();
		m_friends[fr.playerId] = fr;

		setStateDB(target->getId(), 1);

		player->send(SendAcceptIncomingFriend(fr));

		m_incomingFriends.erase(it);
	}
}

void FriendManager::onOutgoingAccepted(std::shared_ptr<Player> target)
{
	if (auto player = m_player.lock(); player != nullptr)
	{
		std::lock_guard<std::mutex> lg(m_mx);

		auto it = m_outgoingFriends.find(target->getId());

		if (it == m_outgoingFriends.cend())
			return;

		Friend fr = it->second;
		fr.state = 1;
		fr.isOnline = target->isOnline();
		m_friends[fr.playerId] = fr;

		setStateDB(target->getId(), 1);

		player->send(SendOutgoingFriendAccepted(fr));

		m_outgoingFriends.erase(it);
	}
}

void FriendManager::remove(uint32_t playerId)
{
	if (auto player = m_player.lock(); player != nullptr)
	{
		m_mx.lock();
		m_friends.erase(playerId);
		m_mx.unlock();
		
		player->send(SendRemoveFriend(playerId));

		removeFromDB(playerId);
	}
}

void FriendManager::onRemoved(uint32_t playerId)
{
	if (auto player = m_player.lock(); player != nullptr)
	{
		std::lock_guard<std::mutex> lg(m_mx);

		auto it = m_friends.find(playerId);

		if (it == m_friends.cend())
			return;

		Friend fr = (*it).second;

		m_friends.erase(it);

		player->send(SendFriendRemoved(fr));

		removeFromDB(playerId);
	}
}

std::vector<Friend> FriendManager::list()
{
	std::vector<Friend> friends;

	std::lock_guard<std::mutex> lg(m_mx);

	for (auto& [id, fr] : m_friends)
		friends.push_back(fr);

	for (auto& [id, fr] : m_incomingFriends)
		friends.push_back(fr);

	for (auto& [id, fr] : m_outgoingFriends)
		friends.push_back(fr);

	return friends;
}

void FriendManager::setStateDB(uint32_t targetId, uint8_t state)
{
	if (auto player = m_player.lock(); player != nullptr)
		DATABASE_DISPATCHER->dispatch("UPDATE friends SET status = ? WHERE player_from = ? AND player_to = ?", { state, player->getId(), targetId });
}

void FriendManager::removeFromDB(uint32_t playerId)
{
	if (auto player = m_player.lock(); player != nullptr)
		DATABASE_DISPATCHER->dispatch("DELETE FROM friends WHERE player_from = ? AND player_to = ?", { player->getId(), playerId });
}

void FriendManager::close()
{
	appearOffline();
}
