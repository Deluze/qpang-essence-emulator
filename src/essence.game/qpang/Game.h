#pragma once

#include <cstdint>

#include <unordered_map>
#include <mutex>

#include "core/Emulator.h"
#include "core/communication/server/QpangServer.h"
#include "core/database/DatabaseDispatcher.h"

#include "qpang/channel/ChannelManager.h"
#include "qpang/account/BanManager.h"
#include "qpang/shop/ShopManager.h"
#include "qpang/square/SquareManager.h"
#include "qpang/cache/CacheManager.h"
#include "qpang/chat/ChatManager.h"
#include "qpang/room/RoomManager.h"
#include "qpang/weapon/WeaponManager.h"
#include "qpang/spawn/SpawnManager.h"
#include "qpang/skill/SkillManager.h"
#include "qpang/achievement/AchievementManager.h"
#include "qpang/level/LevelManager.h"
#include "qpang/crane/CraneManager.h"
#include "qpang/leaderboard/Leaderboard.h"

#include "qpang/room/RoomServer.h"

#include "packets/lobby/LobbyPacketHandler.h"
#include "packets/square/SquarePacketHandler.h"

#include "qpang/ItemID.h"

class Player;

class Game
{
public:
	static Game* instance()
	{
		static Game game;
		return &game;
	}

	void test();
	
	void initialize();
	void run();

	void tick();

	void removeClient(Player::Ptr player);
	void createPlayer(QpangConnection::Ptr conn, uint32_t playerId);

	std::shared_ptr<Player> getPlayer(uint32_t playerId);
	std::shared_ptr<Player> getPlayer(const std::u16string& nickname);

	std::shared_ptr<Player> getOnlinePlayer(uint32_t playerId);
	std::shared_ptr<Player> getOnlinePlayer(const std::u16string& nickname);
	
	std::unordered_map<uint32_t, std::shared_ptr<Player>> getPlayers();

	void broadcast(const std::u16string& message);
	void send(LobbyServerPacket& pack);

	BanManager* getBanManager();
	ChannelManager* getChannelManager();
	ShopManager* getShopManager();
	SquareManager* getSquareManager();
	CacheManager* getCacheManager();
	ChatManager* getChatManager();
	RoomManager* getRoomManager();
	RoomServer* getRoomServer();
	WeaponManager* getWeaponManager();
	SpawnManager* getSpawnManager();
	SkillManager* getSkillManager();
	AchievementManager* getAchievementManager();
	LevelManager* getLevelManager();
	Leaderboard* getLeaderboard();
	CraneManager* getCraneManager();

	DatabaseDispatcher* getDatabaseDispatcher();

private:
	void onLobbyConnectionClosed(QpangConnection::Ptr conn);
	void onLobbyConnection(QpangConnection::Ptr conn);

	void onSquareConnectionClosed(QpangConnection::Ptr conn);
	void onSquareConnection(QpangConnection::Ptr conn);

	QpangServer* m_lobbyServer;
	QpangServer* m_squareServer;
	RoomServer m_roomServer;

	DatabaseDispatcher m_dbDispatcher;

	LobbyPacketHandler m_lobbyPacketHandler;
	SquarePacketHandler m_squarePacketHandler;

	BanManager m_banManager;
	ChannelManager m_channelManager;
	ShopManager m_shopManager;
	SquareManager m_squareManager;
	CacheManager m_cacheManager;
	ChatManager m_chatManager;
	RoomManager m_roomManager;
	WeaponManager m_weaponManager;
	SpawnManager m_spawnManager;
	SkillManager m_skillManager;
	AchievementManager m_achievementManager;
	LevelManager m_levelManager;
	CraneManager m_craneManager;

	Leaderboard m_leaderboard;

	std::recursive_mutex m_playerMx;

	std::unordered_map<uint32_t, std::shared_ptr<Player>> m_players;
	std::unordered_map<std::u16string, std::shared_ptr<Player>> m_playersByNickname;

	
};

#define DATABASE_DISPATCHER Game::instance()->getDatabaseDispatcher()