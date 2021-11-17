#include "Player.h"

#include <ctime>
#include <exception>

#include "core/database/Database.h"

#include "qpang/Game.h"
#include "qpang/cache/CacheManager.h"
#include "qpang/square/SquareManager.h"
#include "qpang/room/tnl/GameConnection.h"

#include "packets/square/outgoing/JoinSquareSuccess.h"
#include "packets/square/outgoing/UpdatePlayerLevel.h"
#include "packets/lobby/outgoing/player/SendUpdatePlayerCharacter.h"
#include "packets/lobby/outgoing/player/SendPlayerReceiveWhisper.h"
#include "packets/lobby/outgoing/Broadcast.h"

Player::Player(uint32_t playerId) :
	m_playerId(playerId),
	m_loginTime(time(NULL))
{
}

void Player::initialize()
{
	std::lock_guard<std::mutex> g(m_mx);

	{
		auto stmt = DATABASE->prepare("SELECT * FROM players WHERE id = ?");
		stmt->bindInteger(m_playerId);
		auto res = stmt->fetch();

		if (!res->hasResults())
		{
			m_exists = false;
			return;
		}

		m_userId = res->getInt("user_id");
		m_character = res->getShort("default_character");
		m_rank = res->getTiny("rank");
		m_prestige = res->getTiny("prestige");
		m_level = res->getShort("level");
		m_name = res->getString("name");
		m_don = res->getInt("don");
		m_cash = res->getInt("cash");
		m_coins = res->getInt("coins");
		m_experience = res->getInt("experience");
		m_isMuted = res->getTiny("is_muted") == 1;
		m_isPatreon = res->getTiny("is_patreon") == 1;
	}

	m_inventoryManager.initialize(shared_from_this(), m_playerId);
	m_equipmentManager.initialize(shared_from_this(), m_playerId);
	m_friendManager.initialize(shared_from_this(), m_playerId);
	m_memoManager.initialize(shared_from_this(), m_playerId);
	m_statsManager.initialize(shared_from_this(), m_playerId);
	m_achievementContainer.initialize(m_playerId);

	m_isOnline = true;
}

void Player::setLobbyConn(QpangConnection::Ptr conn)
{
	m_lobbyConn = conn;
}

void Player::setSquareConn(QpangConnection::Ptr conn)
{
	m_squareConn = conn;
}

void Player::broadcast(const std::u16string& message) const
{
	Broadcast br(message);

	if (const auto lobbyConn = m_lobbyConn.lock(); lobbyConn != nullptr) {
		lobbyConn->send(br);
	}
}

void Player::send(const SquareServerPacket& packet) const
{
	if (const auto squareConn = m_squareConn.lock(); squareConn != nullptr)
		squareConn->send(packet);
}

void Player::send(const LobbyServerPacket& packet) const
{
	if (const auto lobbyConn = m_lobbyConn.lock(); lobbyConn != nullptr)
		lobbyConn->send(packet);
}

void Player::close()
{
	m_mx.lock();

	if (m_isClosed)
	{
		m_mx.unlock();
		return;
	}

	m_isOnline = false;
	m_isClosed = true;

	if (auto lobbyConn = m_lobbyConn.lock(); lobbyConn != nullptr)
		lobbyConn->close();

	if (auto squareConn = m_squareConn.lock(); squareConn != nullptr)
		squareConn->close();

	m_mx.unlock();

	setOnlineStatus(false);

	m_inventoryManager.close();
	m_equipmentManager.close();
	m_friendManager.close();

	if (const auto squarePlayer = m_squarePlayer.lock(); squarePlayer != nullptr)
		squarePlayer->getSquare()->remove(m_playerId);

	m_lobbyConn.reset();
	m_squareConn.reset();

	update();
}

bool Player::isClosed()
{
	return m_isClosed;
}

void Player::whisper(const std::u16string& nickname, const std::u16string& message) const
{
	send(SendPlayerReceiveWhisper(nickname, message));
}

void Player::enterSquare(std::shared_ptr<SquarePlayer> squarePlayer)
{
	assert(squarePlayer != nullptr);

	m_squarePlayer = squarePlayer;

	setOnlineStatus();
	send(JoinSquareSuccess(squarePlayer));
}

void Player::leaveSquare()
{
	m_squarePlayer.reset();
}

void Player::setRoomPlayer(std::shared_ptr<RoomPlayer> roomPlayer)
{
	m_roomPlayer = roomPlayer;
}

void Player::setName(std::u16string name)
{
	m_name = name;

	update();
}

void Player::update()
{
	DATABASE_DISPATCHER->dispatch(
		"UPDATE players SET default_character = ?, don = ?, cash = ?, coins = ?, level = ?, prestige = ?, experience = ?, name = ? WHERE id = ?",
		{
			m_character,
			m_don,
			m_cash,
			m_coins,
			m_level,
			m_prestige,
			m_experience,
			m_name,
			m_playerId,
		}
	);
}

void Player::apply(std::shared_ptr<RoomSessionPlayer> session)
{
}

void Player::ban(time_t until, uint32_t bannedByUserId)
{
	const auto currTime = time(NULL);
	DATABASE_DISPATCHER->dispatch("INSERT INTO user_bans (`user_id`, `timestamp_ban`, `timestamp_unban`, `banned_by_user_id`) VALUES (?, ?, ?, ?)", {
		m_userId,
		static_cast<uint64_t>(currTime),
		static_cast<uint64_t>(until),
		bannedByUserId
		});

	close();
}

InventoryManager* Player::getInventoryManager()
{
	return &m_inventoryManager;
}

EquipmentManager* Player::getEquipmentManager()
{
	return &m_equipmentManager;
}

FriendManager* Player::getFriendManager()
{
	return &m_friendManager;
}

MemoManager* Player::getMemoManager()
{
	return &m_memoManager;
}

StatsManager* Player::getStatsManager()
{
	return &m_statsManager;
}

AchievementContainer* Player::getAchievementContainer()
{
	return &m_achievementContainer;
}

uint32_t Player::getId()
{
	return m_playerId;
}

uint32_t Player::getUserId()
{
	return m_userId;
}

std::u16string Player::getName()
{
	return m_name;
}

uint8_t Player::getRank()
{
	return m_rank;
}

uint32_t Player::getExperience()
{
	return m_experience;
}

void Player::addExperience(uint32_t experience)
{
	m_experience += experience;
}

uint8_t Player::getLevel()
{
	return m_level;
}

void Player::setLevel(uint8_t level)
{
	m_level = level;

	if (auto squarePlayer = m_squarePlayer.lock(); squarePlayer != nullptr)
		squarePlayer->getSquare()->sendPacket(UpdatePlayerLevel{ m_playerId, m_level });
}

uint8_t Player::getPrestige()
{
	return m_prestige;
}

void Player::setCharacter(uint16_t character)
{
	m_character = character;

	send(SendUpdatePlayerCharacter(character));
}

uint16_t Player::getCharacter()
{
	return m_character;
}

bool Player::isOnline()
{
	return m_isOnline;
}

void Player::setOnlineStatus(bool online)
{
	m_isOnline = online;

	if (online)
		m_friendManager.appearOnline();
	else
		m_friendManager.appearOffline();
}

uint32_t Player::getDon()
{
	return m_don;
}

void Player::removeDon(uint32_t val)
{
	if (m_don <= val)
		m_don = 0;
	else
		m_don -= val;

	update();
}

void Player::addDon(uint32_t val)
{
	m_don += val;

	update();
}

uint32_t Player::getCash()
{
	return m_cash;
}

void Player::removeCash(uint32_t val)
{
	if (m_cash <= val)
		m_cash = 0;
	else
		m_cash -= val;

	update();
}

void Player::addCash(uint32_t val)
{
	m_cash += val;
}

uint32_t Player::getCoins()
{
	return m_coins;
}

void Player::addCoins(uint32_t val)
{
	m_coins += val;

	update();
}

void Player::removeCoins(uint32_t val)
{
	if (m_coins <= val)
		m_coins = 0;
	else
		m_coins -= val;

	update();
}

time_t Player::getLoginTime()
{
	return m_loginTime;
}

void Player::mute()
{
	m_isMuted = true;

	broadcast(u"You have been restricted from chatting by a moderator.");

	DATABASE_DISPATCHER->dispatch("UPDATE `players` SET `is_muted` = 1 WHERE `id` = ?", { m_playerId });
}

void Player::unmute()
{
	m_isMuted = false;

	broadcast(u"Your restriction from talking in chat has been lifted.");

	DATABASE_DISPATCHER->dispatch("UPDATE `players` SET `is_muted` = 0 WHERE `id` = ?", { m_playerId });
}

bool Player::isMuted()
{
	return m_isMuted;
}

bool Player::isPatreon() 
{
	return m_isPatreon;
}

bool Player::exists()
{
	return m_exists;
}

std::shared_ptr<SquarePlayer> Player::getSquarePlayer()
{
	return m_squarePlayer.lock();
}

std::shared_ptr<RoomPlayer> Player::getRoomPlayer()
{
	return m_roomPlayer.lock();
}
