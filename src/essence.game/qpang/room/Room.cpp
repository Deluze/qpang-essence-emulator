#include "Room.h"

#include <algorithm>

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/room/RoomManager.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/tnl/GameConnection.h"
#include "qpang/room/tnl/net_events/client/cg_exit.hpp"
#include "qpang/room/tnl/net_events/client/cg_room.hpp"
#include "qpang/room/tnl/net_events/server/gc_exit.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"
#include "qpang/room/tnl/net_events/server/gc_join.hpp"
#include "qpang/room/tnl/net_events/server/gc_room.hpp"

Room::Room(const uint32_t id, const std::u16string name, const uint8_t map, const uint8_t mode, const uint32_t host,
	const uint16_t port) :
	m_host(host),
	m_port(port),
	m_id(id),
	m_name(name),
	m_map(map),
	m_mode(mode),
	m_state(2),
	m_maxPlayers(16),
	m_masterPlayerId(0),
	m_isLevelLimited(false),
	m_isTeamSorting(false),
	m_isSkillsEnabled(true),
	m_isMeleeOnly(false),
	m_scorePoints(40),
	m_scoreTime(10),
	m_isPointsGame(false),
	m_isPlaying(false),
	m_isEventRoom(false)
{
	m_modeManager = Game::instance()->getRoomManager()->getGameModeManager()->get(mode);
	m_isReloadGlitchEnabled = false;

	setSkillsEnabled(!m_modeManager->isPublicEnemyMode());
}

void Room::addPlayer(GameConnection* conn)
{
	if (conn == nullptr || conn->getPlayer() == nullptr)
	{
		return;
	}

	conn->incRef();

	const auto roomPlayer = std::make_shared<RoomPlayer>(conn, shared_from_this());

	conn->getPlayer()->setRoomPlayer(roomPlayer);

	if (m_players.empty())
	{
		m_masterPlayerId = roomPlayer->getPlayer()->getId();
	}

	roomPlayer->setTeam(getAvailableTeam());

	m_playerMx.lock();
	m_players[conn->getPlayer()->getId()] = roomPlayer;
	m_playerMx.unlock();

	conn->enterRoom(shared_from_this());

	syncPlayers(roomPlayer);

	conn->decRef();
}

void Room::removePlayer(uint32_t id)
{
	std::lock_guard lg(m_playerMx);

	const auto it = m_players.find(id);

	if (it == m_players.cend())
	{
		return;
	}

	m_players.erase(it);

	if (id == m_masterPlayerId)
	{
		m_masterPlayerId = findNewMaster();
	}

	if (m_roomSession != nullptr)
	{
		m_roomSession->removePlayer(id);
	}

	broadcastWaiting<GCExit>(id, CGExit::Command::LEAVE, m_masterPlayerId);

	if (m_masterPlayerId == NULL)
	{
		if (!m_players.empty())
		{
			for (const auto& [id, player] : m_players)
			{
				player->getConnection()->disconnect("room closed");
			}
		}

		Game::instance()->getRoomManager()->remove(m_id);
	}
}

std::shared_ptr<RoomPlayer> Room::getPlayer(uint32_t id)
{
	std::lock_guard lg(m_playerMx);

	auto it = m_players.find(id);

	if (it == m_players.cend())
	{
		return nullptr;
	}

	return (*it).second;
}

std::shared_ptr<RoomSession> Room::getRoomSession() const
{
	return m_roomSession;
}

void Room::tick() const
{
	if (m_isPlaying && m_roomSession != nullptr)
	{
		m_roomSession->tick();
	}
}

void Room::start()
{
	if (m_isPlaying)
	{
		return;
	}

	const auto curr = shared_from_this();

	m_roomSession = std::make_shared<RoomSession>(curr, m_modeManager);

	m_roomSession->initialize();

	m_isPlaying = true;
	m_state = 64;

	m_playerMx.lock();

	for (const auto& [id, player] : m_players)
	{
		if (player->isReady() || m_masterPlayerId == id)
		{
			player->setReady(true); // room master can't ready up.
			player->setPlaying(true);
			player->getConnection()->startLoading(curr, player);
			player->onStart();
		}
		else
		{
			player->getConnection()->startGameButNotReady();
		}
	}

	m_playerMx.unlock();
}

void Room::close()
{
	std::lock_guard lg(m_playerMx);

	for (const auto& [id, player] : m_players)
	{
		player->getConnection()->disconnect("room closed");
	}

	Game::instance()->getRoomManager()->remove(m_id);
}

void Room::update(const uint32_t cmd, const uint32_t val)
{
	std::lock_guard lg(m_playerMx);

	for (const auto& [id, roomPlayer] : m_players)
	{
		if (!roomPlayer->isPlaying())
		{
			roomPlayer->getConnection()->postNetEvent(new GCRoom(id, cmd, val, shared_from_this()));
		}
	}
}

void Room::finish()
{
	m_roomSession->clear();
	m_roomSession.reset();

	m_state = 2;
	m_isPlaying = false;

	unreadyAll();
}

void Room::syncPlayers(const std::shared_ptr<RoomPlayer> player)
{
	std::lock_guard lg(m_playerMx);

	player->getConnection()->postNetEvent(new GCJoin(player));

	for (const auto& [id, targetRoomPlayer] : m_players)
	{
		if (player != targetRoomPlayer)
		{
			if (!targetRoomPlayer->isPlaying())
				targetRoomPlayer->getConnection()->postNetEvent(new GCJoin(player));

			player->getConnection()->postNetEvent(new GCJoin(targetRoomPlayer));
		}
	}
}

void Room::balancePlayers()
{
	std::lock_guard lg(m_playerMx);

	for (const auto& [id, roomPlayer] : m_players)
	{
		if (!m_modeManager->isTeamMode())
		{
			roomPlayer->setTeam(0);
		}
		else
		{
			if (roomPlayer->getTeam() != 0)
			{
				continue;
			}

			const auto availableTeam = getAvailableTeam();

			roomPlayer->setTeam(getAvailableTeam());
		}
	}
}

bool Room::canStartInTeam(const uint8_t team) const
{
	if (m_roomSession == nullptr)
	{
		return true;
	}

	if (!m_roomSession->getGameMode()->isTeamMode())
	{
		return true;
	}

	const auto bluePlayers = m_roomSession->getPlayersForTeam(1);
	const auto yellowPlayers = m_roomSession->getPlayersForTeam(2);

	if (team == 1) // blue
	{
		if (bluePlayers.empty())
		{
			return true;
		}

		if (bluePlayers.size() - 1 >= yellowPlayers.size())
		{
			return false;
		}
	}
	else if (team == 2)// yellow
	{
		if (yellowPlayers.empty())
		{
			return true;
		}

		if (yellowPlayers.size() - 1 >= bluePlayers.size())
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

uint32_t Room::getHost() const
{
	return m_host;
}

uint16_t Room::getPort() const
{
	return m_port;
}

std::unordered_map<uint32_t, std::shared_ptr<RoomPlayer>> Room::getPlayers() const
{
	return m_players;
}

uint32_t Room::getId() const
{
	return m_id;
}

std::u16string Room::getName() const
{
	return m_name;
}

uint8_t Room::getMap() const
{
	return m_map;
}

void Room::setMap(const uint8_t map)
{
	if (map > 12)
	{
		return;
	}

	m_map = map;

	update(CGRoom::Command::MAP_ROOM, map);
}

uint8_t Room::getMode() const
{
	return m_mode;
}

void Room::setMode(const uint8_t mode)
{
	m_mode = mode;

	m_modeManager = Game::instance()->getRoomManager()->getGameModeManager()->get(mode);

	m_modeManager->onApply(shared_from_this());

	unreadyAll(true);
	update(CGRoom::Command::MODE_ROOM, mode);

	// after changing game modes, these reset.

	if (m_isPointsGame)
	{
		update(CGRoom::Command::SET_POINTS, m_scorePoints);
	}
	else
	{
		update(CGRoom::Command::SET_TIME, m_scoreTime);
	}
}

GameMode* Room::getModeManager() const
{
	return m_modeManager;
}

uint8_t Room::getState() const
{
	return m_state;
}

void Room::setState(const uint8_t state)
{
	m_state = state;
}

uint8_t Room::getPlayerCount() const
{
	return static_cast<uint8_t>(m_players.size());
}

uint8_t Room::getMaxPlayers() const
{
	return m_maxPlayers;
}

void Room::setMaxPlayers(const uint8_t maxPlayers)
{
	m_maxPlayers = maxPlayers;

	update(CGRoom::Command::PLAYERS_ROOM, maxPlayers);
}

bool Room::isLevelLimited() const
{
	return m_isLevelLimited;
}

void Room::setLevelLimited(const bool levelLimited)
{
	m_isLevelLimited = levelLimited;

	update(CGRoom::Command::LEVEL_ROOM, levelLimited);
}

bool Room::isTeamSorting() const
{
	return m_isTeamSorting;
}

void Room::setTeamSorting(const bool teamSorting)
{
	m_isTeamSorting = teamSorting;

	update(CGRoom::Command::TEAM_ROOM, teamSorting);
}

// TODO: Rename to "areSkillsEnabled"
bool Room::isSkillsEnabled() const
{
	// TODO: Rename to "m_areSkillsEnabled"
	return m_isSkillsEnabled;
}

void Room::setSkillsEnabled(const bool skillEnabled)
{
	m_isSkillsEnabled = skillEnabled;

	unreadyAll(true);
	update(CGRoom::Command::TOGGLE_SKILL, skillEnabled);
}

bool Room::isMeleeOnly() const
{
	return m_isMeleeOnly;
}

void Room::setMeleeOnly(const bool meleeOnly)
{
	m_isMeleeOnly = meleeOnly;

	if (meleeOnly)
	{
		m_isSkillsEnabled = false;
	}

	unreadyAll(true);
	update(CGRoom::Command::TOGGLE_MELEE, meleeOnly);
}

uint32_t Room::getMasterId() const
{
	return m_masterPlayerId;
}

uint32_t Room::findNewMaster()
{
	std::lock_guard lg(m_playerMx);

	if (m_players.empty())
	{
		return 0;
	}

	return m_players.begin()->first;
}

uint8_t Room::getAvailableTeam()
{
	if (!m_modeManager->isTeamMode())
	{
		return 0;
	}

	m_playerMx.lock();

	const size_t yellowCount = std::count_if(m_players.cbegin(), m_players.cend(),
		[](const std::pair<uint32_t, RoomPlayer::Ptr>& pair) {
			return pair.second->getTeam() == 2;
		});

	m_playerMx.unlock();

	if (yellowCount * 2 >= m_players.size())
	{
		return 1;
	}

	return 2;
}

bool Room::isTeamAvailable(uint8_t team)
{
	std::lock_guard lg(m_playerMx);

	if (team != 1 && team != 2)
	{
		return false;
	}

	m_playerMx.lock();

	const size_t teamCount = std::count_if(m_players.cbegin(), m_players.cend(),
		[team](const std::pair<uint32_t, RoomPlayer::Ptr> pair) {
			return pair.second->getTeam() == team;
		});

	m_playerMx.unlock();

	return teamCount * 2 < m_maxPlayers;
}

uint32_t Room::getScorePoints() const
{
	return m_scorePoints;
}

void Room::setScorePoints(const uint32_t points)
{
	m_scorePoints = points;

	update(CGRoom::Command::SET_POINTS, points);
}

uint32_t Room::getScoreTime() const
{
	return m_scoreTime;
}

void Room::setScoreTime(const uint32_t minutes)
{
	m_scoreTime = minutes;

	update(CGRoom::Command::SET_TIME, minutes);
}

bool Room::isPointsGame() const
{
	return m_isPointsGame;
}

void Room::setIsPointsGame(const bool isPoints)
{
	m_isPointsGame = isPoints;
}

bool Room::isPlaying() const
{
	return m_isPlaying;
}

void Room::unreadyAll(const bool notify)
{
	std::lock_guard lg(m_playerMx);

	for (const auto& [id, player] : m_players)
	{
		if (player->isReady())
		{
			player->setReady(false);

			if (notify)
			{
				player->getPlayer()->broadcast(u"Your ready status has been removed because the room rules have changed.");
			}
		}

		if (player->isPlaying())
		{
			player->setPlaying(false);
			player->setSpectating(false);
		}
	}
}

std::string Room::getPassword() const
{
	return m_password;
}

void Room::setPassword(std::string& password)
{
	if (password.size() > 4)
	{
		password.resize(4);
	}

	m_password = password;

	update(CGRoom::Command::PASS_ROOM, 0);
}

bool Room::isEventRoom() const
{
	return m_isEventRoom;
}

void Room::setEventRoom(const bool isEventRoom)
{
	m_isEventRoom = isEventRoom;
}

bool Room::isReloadGlitchEnabled() const
{
	return m_isReloadGlitchEnabled;
}

void Room::setIsReloadGlitchEnabled(const bool isReloadGlitchEnabled)
{
	m_isReloadGlitchEnabled = isReloadGlitchEnabled;
}