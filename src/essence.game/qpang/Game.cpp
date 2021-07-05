#include "Game.h"

#include <boost/thread.hpp>
#include <any>

#include "packets/lobby/outgoing/account/DuplicateLogin.h"

#include "utils/StringConverter.h"

#include "qpang/player/Player.h"

Square::Ptr square = std::make_shared<Square>(1, u"test");

void thread1()
{
	std::vector<Player::Ptr> players;

	
}

void thread2()
{
	std::vector<Player::Ptr> players;

	
}

void Game::test()
{
	boost::thread a(thread1);
	boost::thread b(thread2);

	a.join();
	b.join();
}

void Game::initialize()
{
	m_lobbyServer = new QpangServer(8005);
	m_squareServer = new QpangServer(8012);

	m_lobbyServer->setAcceptHandler(std::bind(&Game::onLobbyConnection, Game::instance(), std::placeholders::_1));
	m_squareServer->setAcceptHandler(std::bind(&Game::onSquareConnection, Game::instance(), std::placeholders::_1));

	Emulator::instance()->attachServer(m_lobbyServer);
	Emulator::instance()->attachServer(m_squareServer);

	m_channelManager.initialize();
	m_shopManager.initialize();
	m_chatManager.initialize();
	m_weaponManager.initialize();
	m_spawnManager.initialize();
	m_skillManager.initialize();
	m_levelManager.initialize();
	m_craneManager.initialize();
	m_leaderboard.refresh();

	m_roomServer.initialize();
}

void Game::run()
{
	boost::thread t(&RoomServer::run, &m_roomServer);
	boost::thread t3(&Game::tick, Game::instance());
}

void Game::tick()
{
	while (true)
	{
		try
		{
			boost::thread t2(&DatabaseDispatcher::run, &m_dbDispatcher);
			t2.join();
			std::cout << "DatabaseDispatcher exited.\n";
		}
		catch (const std::exception& e)
		{
			std::cout << "An exception occurred: " << e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "FATAL uncaught exception.\n";
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Game::removeClient(Player::Ptr player)
{
	assert(player != nullptr);
	
	m_playerMx.lock();
	m_players.erase(player->getId());
	m_playersByNickname.erase(player->getName());
	m_playerMx.unlock();

	m_cacheManager.getPlayerCacheManager()->cache(player);
}

void Game::createPlayer(QpangConnection::Ptr conn, uint32_t playerId)
{	
	auto player = std::make_shared<Player>(playerId);

	player->setLobbyConn(conn);
	conn->setPlayer(player);

	std::lock_guard<std::recursive_mutex> lg(m_playerMx);
	
	const auto it = m_players.find(playerId);
	const auto playerFound = it != m_players.cend();

	if (playerFound)
	{
		it->second->send(DuplicateLogin());
		it->second->close();
	}

	m_cacheManager.getPlayerCacheManager()->invalidate(playerId);

	player->initialize();

	m_players[playerId] = player;
	m_playersByNickname[StringConverter::ToLowerCase(player->getName())] = player;
}

Player::Ptr Game::getPlayer(uint32_t playerId)
{
	if (auto player = getOnlinePlayer(playerId); player != nullptr)
		return player;

	auto cachedPlayer = m_cacheManager.getPlayerCacheManager()->tryGet(playerId);

	if (cachedPlayer != nullptr)
		return cachedPlayer;

	return m_cacheManager.getPlayerCacheManager()->cache(playerId);
}

Player::Ptr Game::getPlayer(const std::u16string& nickname)
{
	if (auto player = getOnlinePlayer(nickname); player != nullptr)
		return player;

	auto cachedPlayer = m_cacheManager.getPlayerCacheManager()->tryGet(nickname);

	if (cachedPlayer != nullptr)
		return cachedPlayer;

	return m_cacheManager.getPlayerCacheManager()->cache(nickname);
}

Player::Ptr Game::getOnlinePlayer(uint32_t playerId)
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	auto it = m_players.find(playerId);

	return it != m_players.cend() ? it->second : nullptr;
}

Player::Ptr Game::getOnlinePlayer(const std::u16string& nickname)
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	auto it = m_playersByNickname.find(StringConverter::ToLowerCase(nickname));

	return it != m_playersByNickname.cend() ? it->second : nullptr;
}

void Game::broadcast(const std::u16string& message)
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	for (const auto& [id, player] : m_players)
		player->broadcast(message);
}

void Game::send(LobbyServerPacket& pack)
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	for (const auto& [id, player] : m_players)
		player->send(pack);
}

BanManager* Game::getBanManager()
{
	return &m_banManager;
}

ChannelManager* Game::getChannelManager()
{
	return &m_channelManager;
}

ShopManager* Game::getShopManager()
{
	return &m_shopManager;
}

SquareManager* Game::getSquareManager()
{
	return &m_squareManager;
}

CacheManager* Game::getCacheManager()
{
	return &m_cacheManager;
}

ChatManager* Game::getChatManager()
{
	return &m_chatManager;
}

RoomManager* Game::getRoomManager()
{
	return &m_roomManager;
}

RoomServer* Game::getRoomServer()
{
	return &m_roomServer;
}

WeaponManager* Game::getWeaponManager()
{
	return &m_weaponManager;
}

SpawnManager* Game::getSpawnManager()
{
	return &m_spawnManager;
}

SkillManager* Game::getSkillManager()
{
	return &m_skillManager;
}

AchievementManager* Game::getAchievementManager()
{
	return &m_achievementManager;
}

LevelManager* Game::getLevelManager()
{
	return &m_levelManager;
}

Leaderboard* Game::getLeaderboard()
{
	return &m_leaderboard;
}

CraneManager* Game::getCraneManager()
{
	return &m_craneManager;
}

DatabaseDispatcher* Game::getDatabaseDispatcher()
{
	return &m_dbDispatcher;
}

void Game::onLobbyConnectionClosed(QpangConnection::Ptr conn)
{
	m_lobbyServer->removeConnection(conn);

	if (auto player = conn->getPlayer(); player != nullptr)
	{
		if (player->isClosed())
			return;

		player->close();

		removeClient(player);
	}
}

void Game::onLobbyConnection(QpangConnection::Ptr conn)
{
	conn->setPacketHandler(&m_lobbyPacketHandler);
	conn->setDisconnectHandler(std::bind(&Game::onLobbyConnectionClosed, Game::instance(), std::placeholders::_1));
}

void Game::onSquareConnectionClosed(QpangConnection::Ptr conn)
{
	m_squareServer->removeConnection(conn);

	if (auto player = conn->getPlayer(); player != nullptr)
	{
		if (player->isClosed())
			return;

		player->close();

		removeClient(player);
	}
}

void Game::onSquareConnection(QpangConnection::Ptr conn)
{
	conn->setPacketHandler(&m_squarePacketHandler);
	conn->setDisconnectHandler(std::bind(&Game::onSquareConnectionClosed, Game::instance(), std::placeholders::_1));
}
