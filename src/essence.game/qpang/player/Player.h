#pragma once

#include <cstdint>
#include <memory>

#include "core/communication/client/QpangConnection.h"

#include "qpang/player/inventory/InventoryManager.h"
#include "qpang/player/equipment/EquipmentManager.h"
#include "qpang/player/friend/FriendManager.h"
#include "qpang/player/memo/MemoManager.h"
#include "qpang/player/stats/StatsManager.h"
#include "qpang/player/achievement/AchievementContainer.h"

#include "packets/LobbyServerPacket.h"
#include "packets/SquareServerPacket.h"
#include "qpang/square/SquarePlayer.h"

class RoomPlayer;
class GameConnection;
class SquarePlayer;

class Player : public std::enable_shared_from_this<Player>
{
public:

	using Ptr = std::shared_ptr<Player>;

	Player(uint32_t playerId);

	void initialize();
	void setLobbyConn(QpangConnection::Ptr conn);
	void setSquareConn(QpangConnection::Ptr conn);
	
	void broadcast(const std::u16string& message) const;
	void send(const SquareServerPacket& packet) const;
	void send(const LobbyServerPacket& packet) const;

	void close();
	bool isClosed();

	void whisper(const std::u16string& nickname, const std::u16string& message) const;

	void enterSquare(std::shared_ptr<SquarePlayer> square);
	void leaveSquare();

	void setRoomPlayer(std::shared_ptr<RoomPlayer> roomPlayer);
	
	void update();
	void apply(std::shared_ptr<RoomSessionPlayer> session);

	void ban(time_t until, uint32_t bannedByUserId);

	InventoryManager* getInventoryManager();
	EquipmentManager* getEquipmentManager();
	FriendManager* getFriendManager();
	MemoManager* getMemoManager();
	StatsManager* getStatsManager();
	AchievementContainer* getAchievementContainer();

	uint32_t getId();
	uint32_t getUserId();
	std::u16string getName();
	void setName(std::u16string name);
	uint8_t getRank();

	uint32_t getExperience();
	void addExperience(uint32_t experience);

	uint8_t getLevel();
	void setLevel(uint8_t level);

	uint8_t getPrestige();

	void setCharacter(uint16_t character);
	uint16_t getCharacter();

	bool isOnline();
	void setOnlineStatus(bool online = true);

	uint32_t getDon();
	void removeDon(uint32_t val);
	void addDon(uint32_t val);

	uint32_t getCash();
	void removeCash(uint32_t val);
	void addCash(uint32_t val);

	uint32_t getCoins();
	void addCoins(uint32_t val);
	void removeCoins(uint32_t val);
	
	time_t getLoginTime();

	void mute();
	void unmute();
	bool isMuted();

	bool isPatreon();

	bool exists();

	std::shared_ptr<SquarePlayer> getSquarePlayer();
	std::shared_ptr<RoomPlayer> getRoomPlayer();
private:
	InventoryManager m_inventoryManager;
	EquipmentManager m_equipmentManager;
	FriendManager m_friendManager;
	MemoManager m_memoManager;
	StatsManager m_statsManager;
	AchievementContainer m_achievementContainer;

	uint32_t m_playerId = 0;
	uint32_t m_userId = 0;
	std::u16string m_name = u"";
	uint8_t m_rank = 0;
	uint32_t m_experience = 0;
	uint8_t m_level = 0;
	uint8_t m_prestige = 0;
	uint16_t m_character = 333;
	uint32_t m_don = 0;
	uint32_t m_cash = 0;
	uint32_t m_coins = 0;
	uint32_t m_playTime = 0;
	time_t m_loginTime = 0;
	uint32_t m_slackerPoints = 0;

	// new
	bool m_isPatreon = false;

	bool m_isMuted = false;
	bool m_exists = true;
	bool m_isClosed = false;
	bool m_isOnline = false;

	uint32_t m_currentSquareId = 0;

	std::weak_ptr<QpangConnection> m_lobbyConn;
	std::weak_ptr<QpangConnection> m_squareConn;

	std::weak_ptr<SquarePlayer> m_squarePlayer;
	std::weak_ptr<RoomPlayer> m_roomPlayer;

	std::mutex m_mx;
};