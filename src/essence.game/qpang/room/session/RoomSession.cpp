#include "RoomSession.h"

#include <numeric>
#include <chrono>

#include "qpang/room/RoomPlayer.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"
#include "qpang/room/tnl/net_events/server/gc_respawn.hpp"
#include "qpang/room/tnl/net_events/server/gc_score_result.hpp"
#include "qpang/room/tnl/net_events/server/gc_weapon.hpp"
#include "qpang/room/tnl/net_events/server/gc_hit_essence.hpp"

RoomSession::RoomSession(std::shared_ptr<Room> room, GameMode* mode) :
	m_room(room),
	m_gameMode(mode),
	m_isFinished(false),
	m_essenceHolder(nullptr),
	m_bluePoints(0),
	m_yellowPoints(0),
	m_lastTickTime(NULL),
	m_essenceDropTime(NULL),
	m_isEssenceReset(true),
	m_blueVip(nullptr),
	m_nextBlueVip(nullptr),
	m_blueVipSetTime(NULL),
	m_yellowVip(nullptr),
	m_nextYellowVip(nullptr),
	m_yellowVipSetTime(NULL)
{
	m_goal = m_room->isPointsGame() ? m_room->getScorePoints() : m_room->getScoreTime();
	m_isPoints = m_room->isPointsGame();
	m_startTime = time(NULL) + 30 + 5; // 30 (waiting for players) + countdown start
	m_endTime = room->isPointsGame() ? NULL : m_startTime + (static_cast<uint64_t>(room->getScoreTime()) * 60); // additional 30 seconds bcs waiting for players
}

void RoomSession::initialize()
{
	m_itemManager.initialize(shared_from_this());
	//m_skillManager.initialize(shared_from_this());

	m_gameMode->onStart(shared_from_this());

	m_essencePosition = Game::instance()->getSpawnManager()->getEssenceSpawn(m_room->getMap());
}

void RoomSession::addPlayer(GameConnection* conn, uint8_t team)
{
	auto player = std::make_shared<RoomSessionPlayer>(conn, shared_from_this(), team);

	if (auto roomPlayer = conn->getPlayer()->getRoomPlayer(); roomPlayer != nullptr)
		roomPlayer->setRoomSessionPlayer(player);

	player->initialize();
	player->setSpectating(conn->getPlayer()->getRoomPlayer()->isSpectating());

	m_playerMx.lock();
	for (const auto& [id, session] : m_players)
	{
		if(!session->isSpectating())
			player->addPlayer(session);

		if(!player->isSpectating())
			session->addPlayer(player);

		if (session->isPlaying())
			player->post(new GCGameState(id, 3));
	}
	m_players[player->getPlayer()->getId()] = player;
	m_playerMx.unlock();

	auto spawn = Game::instance()->getSpawnManager()->getRandomSpawn(m_room->getMap(), team);
	player->post(new GCGameState(player->getPlayer()->getId(), 11, 0)); // Necessary to initiate spectator mode in "waiting for players" state
	player->post(new GCRespawn(player->getPlayer()->getId(), player->getCharacter(), 1, spawn.x, spawn.y, spawn.z));

	if (!player->isSpectating())
	{
		m_leaverMx.lock();
		auto it = std::find_if(m_leavers.cbegin(), m_leavers.cend(),
			[player](const RoomSessionPlayer::Ptr x)
		{
			return player->getPlayer()->getId() == x->getPlayer()->getId();
		}
		);

		if (it != m_leavers.cend())
			m_leavers.erase(it);

		m_leaverMx.unlock();
	}
}

bool RoomSession::removePlayer(uint32_t playerId)
{
	m_playerMx.lock();

	auto it = m_players.find(playerId);

	if (it == m_players.cend())
	{
		m_playerMx.unlock();
		return false;
	}

	auto player = it->second;

	if (player == m_essenceHolder)
	{
		setEssenceHolder(nullptr);
		const auto& pos = player->getPosition();

		for (const auto& [id, session] : m_players)
			session->post(new GCHitEssence(session->getPlayer()->getId(), player->getPlayer()->getId(), 3, pos.x, pos.y, pos.z, 0, 6));
	}
	
	if (player == m_blueVip)
		setBlueVip(nullptr);
	else if (player == m_yellowVip)
		setYellowVip(nullptr);

	if (player->isPlaying())
		relay<GCGameState>(playerId, 15);
	else
	{
		relayPlaying<GCGameState>(playerId, 15);
		player->post(new GCGameState(playerId, 15));
	}

	m_players.erase(it);
	m_playerMx.unlock();

	if (player->isSpectating())
		return true;

	m_leaverMx.lock();
	m_leavers.push_back(player);
	m_leaverMx.unlock();

	if (!m_isFinished)
	{
		if (m_gameMode->isTeamMode())
		{
			if (m_players.empty())
				finish();
			else
			{
				const auto bluePlayers = getPlayersForTeam(1);
				const auto yellowPlayers = getPlayersForTeam(2);

				if (bluePlayers.empty())
				{
					m_yellowPoints = 1;
					m_bluePoints = 0;
					finish();
				}
				else if (yellowPlayers.empty())
				{
					m_yellowPoints = 0;
					m_bluePoints = 1;
					finish();
				}
			}
		}
		else
			if (m_players.size() <= 1)
				finish();
	}

	return true;
}

RoomSessionPlayer::Ptr RoomSession::find(uint32_t playerId)
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	const auto it = m_players.find(playerId);

	if (it == m_players.cend())
		return nullptr;

	return it->second;
}

RoomSessionPlayer::Ptr RoomSession::findEligibleVip(uint8_t team, bool noConditions)
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);
	std::vector<RoomSessionPlayer::Ptr> players;

	uint32_t playerCount = 0;
	for (const auto&[id, player] : m_players)
	{
		if (player->isSpectating() || (player->getPlayer()->getRank() == 3 && this->getRoom()->isEventRoom()))
			continue;

		if (player->getTeam() == team && (noConditions || (player->isDead() && player != m_blueVip && player != m_yellowVip)))
		{
			players.push_back(player);
			playerCount++;
		}
	}

	if (!playerCount)
		return nullptr;
	auto index = rand() % playerCount;

	return players[index];
}

void RoomSession::handlePlayerFinish(RoomSessionPlayer::Ptr player)
{
	player->getPlayer()->addExperience(player->getExperience());
	player->getPlayer()->addDon(player->getDon());

	const auto isTeamMode = m_gameMode->isTeamMode();
	const auto isMissionMode = m_gameMode->isMissionMode();

	auto statManager = player->getPlayer()->getStatsManager();

	if (isTeamMode)
	{
		const auto yellowPoints = getYellowPoints();
		const auto bluePoints = getBluePoints();

		if (yellowPoints == bluePoints)
		{
			if (isMissionMode)
				statManager->addMissionDraw();
			else
				statManager->addNormalDraw();
		}
		else
		{
			auto didWin = bluePoints > yellowPoints ? player->getTeam() == 1 : player->getTeam() == 2;

			if (didWin)
			{
				if (isMissionMode)
					statManager->addMissionWin();
				else
					statManager->addNormalWin();
			}
			else
				if (isMissionMode)
					statManager->addMissionLoss();
				else
					statManager->addNormalLoss();
		}
	}
	else
	{
		auto players = getPlayers();

		auto winningPlayers = players.size() % 2 == 0 ? players.size() / 2 : (players.size() - 1) / 2;

		std::sort(players.begin(), players.end(),
			[](RoomSessionPlayer::Ptr& lhs, RoomSessionPlayer::Ptr& rhs)
			{
				return lhs->getScore() > rhs->getScore();
			}
		);

		for (size_t i = 0; i < winningPlayers % players.size(); i++)
		{
			if (player->getPlayer()->getId() == players[i]->getPlayer()->getId())
			{
				if (isMissionMode)
					player->getPlayer()->getStatsManager()->addMissionWin();
				else
					player->getPlayer()->getStatsManager()->addNormalWin();
			}
			else if (i == winningPlayers - 1)
			{
				if (isMissionMode)
					player->getPlayer()->getStatsManager()->addMissionLoss();
				else
					player->getPlayer()->getStatsManager()->addNormalLoss();
			}
		}
	}
}

void RoomSession::tick()
{	
	if (!m_isFinished)
	{
		std::unique_lock<std::recursive_mutex> lg(m_playerMx);
		
		m_itemManager.tick();
		m_gameMode->tick(shared_from_this());

		for (auto& [id, session] : m_players)
			session->tick();
	}

	if (canFinish())
		finish();
}

void RoomSession::clear()
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);
	std::lock_guard<std::recursive_mutex> lg2(m_leaverMx);

	m_players.clear();
	m_leavers.clear();
}

bool RoomSession::isFinished()
{
	return m_isFinished;
}

void RoomSession::finish()
{
	if (m_isFinished)
		return;

	m_isFinished = true;

	m_itemManager.reset();
	m_essenceHolder.reset();
	m_nextBlueVip.reset();
	m_blueVip.reset();
	m_nextYellowVip.reset();
	m_yellowVip.reset();

	m_leaverMx.lock();
	for (const auto& player : m_leavers)
	{
		if (m_gameMode->isMissionMode())
			player->getPlayer()->getStatsManager()->addMissionLoss();
		else
			player->getPlayer()->getStatsManager()->addNormalLoss();

		player->stop();
	}
	m_leaverMx.unlock();

	auto players = getPlayers();

	for (const auto& player : players)
	{
		if (!player->isSpectating())
		{
			handlePlayerFinish(player);
			player->stop();
		}
	}

	auto playingPlayers = getPlayingPlayers();

	std::sort(playingPlayers.begin(), playingPlayers.end(),
		[](RoomSessionPlayer::Ptr& lhs, RoomSessionPlayer::Ptr& rhs)
		{
			return lhs->getScore() > rhs->getScore();
		}
	);
	
	for (const auto& player : players)
	{
		const auto actPlayer = player->getPlayer();

		player->post(new GCGameState(actPlayer->getId(), 1));
		player->post(new GCGameState(player, 23));
		player->post(new GCScoreResult(shared_from_this(), playingPlayers));
	}

	m_room->finish();
}

bool RoomSession::canFinish()
{
	if (m_isFinished)
		return false;

	if (m_isPoints)
	{
		if (m_gameMode->isTeamMode())
		{
			auto bluePoints = getBluePoints();
			auto yellowPoints = getYellowPoints();

			return bluePoints >= m_goal || yellowPoints >= m_goal;
		}

		return getTopScore() >= m_goal;
	}

	const auto currTime = time(NULL);

	return currTime >= m_endTime;
}

bool RoomSession::isAlmostFinished()
{
	if (m_isPoints)
	{
		const auto maxPointDiffer = m_goal > 20 ? 10 : 5;

		if (m_gameMode->isTeamMode())
		{
			const auto bluePoints = getBluePoints();
			const auto yellowPoints = getYellowPoints();

			return bluePoints + maxPointDiffer >= m_goal || yellowPoints + maxPointDiffer >= m_goal;
		}

		const auto killLeader = getTopScore();

		return killLeader + maxPointDiffer >= m_goal;
	}

	const auto currTime = time(NULL);

	return currTime + 60 >= m_endTime;

}

void RoomSession::addPointsForTeam(uint8_t team, uint32_t amount)
{
	if (team == 1)
		addBluePoints(amount);
	else
		addYellowPoints(amount);
}

void RoomSession::addBluePoints(uint32_t amount)
{
	m_bluePoints += amount;
}

void RoomSession::addYellowPoints(uint32_t amount)
{
	m_yellowPoints += amount;
}

uint32_t RoomSession::getBluePoints()
{
	return m_bluePoints;
}

uint32_t RoomSession::getYellowPoints()
{
	return m_yellowPoints;
}

uint32_t RoomSession::getPointsForTeam(uint8_t team)
{
	const auto players = getPlayers();

	return std::accumulate(players.cbegin(), players.cend(), 0,
		[team](uint32_t acc, RoomSessionPlayer::Ptr player)
		{
			if (player->getTeam() == team)
				return acc + player->getScore();

			return acc;
		}
	);
}

uint32_t RoomSession::getTopScore()
{
	auto players = getPlayers();

	uint32_t topScore{ 0 };

	for (const auto& player : players)
		if (player->getScore() > topScore)
			topScore = player->getScore();

	return topScore;
}

void RoomSession::resetEssence()
{
	m_isEssenceReset = true;

	const auto& pos = Game::instance()->getSpawnManager()->getEssenceSpawn(m_room->getMap());
	const auto players = getPlayingPlayers();

	setEssencePosition(pos);

	for (const auto& player : players)
		player->post(new GCHitEssence(player->getPlayer()->getId(), player->getPlayer()->getId(), 3, pos.x, pos.y, pos.z, 0, 6));
}

void RoomSession::setEssenceHolder(RoomSessionPlayer::Ptr player)
{
	m_essenceHolder = player;

	if (player != nullptr)
	{
		m_essenceDropTime = NULL;
		m_isEssenceReset = false;
	}
	else
		m_essenceDropTime = time(NULL);
}

void RoomSession::setEssencePosition(const Spawn& spawn)
{
	m_essencePosition = spawn;
}

Spawn RoomSession::getEssencePosition()
{
	return m_essencePosition;
}

RoomSessionPlayer::Ptr RoomSession::getEssenceHolder()
{
	return m_essenceHolder;
}

bool RoomSession::isEssenceDropped()
{
	return m_essenceHolder == nullptr;
}

bool RoomSession::isEssenceReset()
{
	return m_isEssenceReset;
}

uint32_t RoomSession::getElapsedEssenceDropTime()
{
	if (m_essenceDropTime == NULL)
		return 0;

	return time(NULL) - m_essenceDropTime;
}

void RoomSession::findNextBlueVip()
{
	m_nextBlueVip = findEligibleVip(1, false);
}

bool RoomSession::isNextBlueVipEligible()
{
	if (m_nextBlueVip == nullptr)
		return false;
	return m_nextBlueVip->isDead();
}

void RoomSession::setBlueVip(RoomSessionPlayer::Ptr player)
{
	m_blueVip = player;
	m_nextBlueVip = nullptr;

	if (player == nullptr)
		m_blueVipSetTime = NULL;
	else
		m_blueVipSetTime = time(NULL);
}

uint32_t RoomSession::getElapsedBlueVipTime()
{
	if (m_blueVipSetTime == NULL)
		return 0;

	return time(NULL) - m_blueVipSetTime;
}

RoomSessionPlayer::Ptr RoomSession::getBlueVip()
{
	return m_blueVip;
}

void RoomSession::findNextYellowVip()
{
	m_nextYellowVip = findEligibleVip(2, false);
}

bool RoomSession::isNextYellowVipEligible()
{
	if (m_nextYellowVip == nullptr)
		return false;
	return m_nextYellowVip->isDead();
}

void RoomSession::setYellowVip(RoomSessionPlayer::Ptr player)
{
	m_yellowVip = player;
	m_nextYellowVip = nullptr;

	if (player == nullptr)
		m_yellowVipSetTime = NULL;
	else
		m_yellowVipSetTime = time(NULL);
}

uint32_t RoomSession::getElapsedYellowVipTime()
{
	if (m_yellowVipSetTime == NULL)
		return 0;

	return time(NULL) - m_yellowVipSetTime;
}

RoomSessionPlayer::Ptr RoomSession::getYellowVip()
{
	return m_yellowVip;
}

bool RoomSession::isVip(RoomSessionPlayer::Ptr player)
{
	return (player->getTeam() == 1 && player == m_blueVip) || (player->getTeam() == 2 && player == m_yellowVip);
}

void RoomSession::spawnPlayer(RoomSessionPlayer::Ptr player)
{
	player->makeInvincible();
	player->setHealth(player->getDefaultHealth());
	player->getWeaponManager()->reset();

	auto spawn = Game::instance()->getSpawnManager()->getRandomSpawn(m_room->getMap(), player->getTeam());

	// Whether someone becomes VIP or not *has* to be decided during respawn.
	if (this->getRoom()->getMode() == GameMode::Mode::VIP)
	{
		if ((m_blueVip == nullptr && player->getTeam() == 1) || (player == m_nextBlueVip && (getElapsedBlueVipTime() > 100 || m_blueVip->isDead())))
			setBlueVip(player);
		else if ((m_yellowVip == nullptr && player->getTeam() == 2) || (player == m_nextYellowVip && (getElapsedYellowVipTime() > 100 || m_yellowVip->isDead())))
			setYellowVip(player);
	}

	relayPlaying<GCRespawn>(player->getPlayer()->getId(), player->getCharacter(), 0, spawn.x, spawn.y, spawn.z, isVip(player));
}

void RoomSession::syncPlayer(RoomSessionPlayer::Ptr player)
{
	m_playerMx.lock();

	for (auto& [id, session] : m_players)
	{
		if (session == player)
			continue;

		player->post(new GCRespawn(id, session->getCharacter(), 0, 255.0F, 255.0F , 255.0F, isVip(player)));
		player->post(new GCGameState(id, 8));
		auto weapon = session->getWeaponManager()->getSelectedWeapon();
		player->post(new GCWeapon(id, 0, weapon.itemId, 0));
	}

	m_playerMx.unlock();

	m_itemManager.syncPlayer(player);
	m_gameMode->onPlayerSync(player);
}

uint32_t RoomSession::getElapsedTime()
{
	time_t currTime = time(NULL);

	if (currTime <= m_startTime)
		return 0;

	return (currTime - m_startTime) * 1000;
}

uint32_t RoomSession::getTimeLeftInSeconds()
{
	if (m_isPoints)
		return 1;

	const auto currTime = time(NULL);

	if (currTime > m_endTime)
		return 0;

	return m_endTime - currTime;
}

void RoomSession::killPlayer(RoomSessionPlayer::Ptr killer, RoomSessionPlayer::Ptr target, uint32_t weaponId, bool isHeadshot)
{
	relayPlaying<GCGameState>(target->getPlayer()->getId(), isHeadshot ? 28 : 17, weaponId, killer->getPlayer()->getId());
}

GameMode* RoomSession::getGameMode()
{
	return m_gameMode;
}

GameItemManager* RoomSession::getItemManager()
{
	return &m_itemManager;
}

RoomSkillManager* RoomSession::getSkillManager()
{
	return &m_skillManager;
}

Room::Ptr RoomSession::getRoom()
{
	return m_room;
}

std::vector<RoomSessionPlayer::Ptr> RoomSession::getPlayers()
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	std::vector<RoomSessionPlayer::Ptr> players;

	for (const auto& [id, session] : m_players)
		players.push_back(session);

	return players;
}

std::vector<RoomSessionPlayer::Ptr> RoomSession::getPlayingPlayers()
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	std::vector<RoomSessionPlayer::Ptr> players;

	for (const auto& [id, session] : m_players)
		if (session->isPlaying() && !session->isSpectating())
			players.push_back(session);

	return players;
}

std::vector<RoomSessionPlayer::Ptr> RoomSession::getPlayersForTeam(uint8_t team)
{
	std::lock_guard<std::recursive_mutex> lg(m_playerMx);

	std::vector<RoomSessionPlayer::Ptr> players;

	for (const auto& [id, player] : m_players)
		if (player->getTeam() == team && !player->isSpectating())
			players.push_back(player);

	return players;
}
