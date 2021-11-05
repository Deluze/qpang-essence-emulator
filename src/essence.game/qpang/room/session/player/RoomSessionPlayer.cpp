#include "RoomSessionPlayer.h"

#include "cc_user_info.hpp"
#include "ConfigManager.h"

#include "packets/lobby/outgoing/account/UpdateAccount.h"

#include "qpang/ItemId.h"
#include "qpang/Game.h"

#include "qpang/room/tnl/GameConnection.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/player/Player.h"

#include "qpang/room/tnl/net_events/GameNetEvent.h"
#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_item.hpp"

RoomSessionPlayer::RoomSessionPlayer(GameConnection* conn, std::shared_ptr<RoomSession> roomSession, uint8_t team) :
	m_isPlaying(false),
	m_isSpectating(false),
	m_team(team),
	m_isInvincible(false),
	m_expRate(0),
	m_donRate(0),
	m_exp(0),
	m_don(0),
	m_highestStreak(0),
	m_streak(0),
	m_kills(0),
	m_deaths(0),
	m_score(0),
	m_playTime(0),
	m_highestMultiKill(0),
	m_eventItemPickUps(0),
	m_deathsAsTag(0),
	m_deathsByTag(0),
	m_tagKillsAsPlayer(0),
	m_playerKillsAsTag(0),
	m_timeAliveAsTag(0),
	m_damageDealtToTag(0),
	m_damageDealtAsTag(0),
	m_conn(conn),
	m_roomSession(roomSession)
{
	conn->incRef();

	auto player = conn->getPlayer();

	const auto waitingForPlayersTime = CONFIG_MANAGER->getInt("WAITING_FOR_PLAYERS");

	m_joinTime = time(NULL);
	m_startTime = m_joinTime + waitingForPlayersTime;
	m_character = player->getCharacter();

	m_isPermanentlyInvincible = false;

	auto* equipManager = player->getEquipmentManager();

	m_baseHealth = equipManager->getBaseHealth();
	m_bonusHealth = equipManager->getBonusHealth();
	m_health = getDefaultHealth();

	m_armor = equipManager->getArmorItemIdsByCharacter(m_character);

	m_hasQuickRevive = equipManager->hasFunctionCard(QUICK_REVIVE);

	m_expRate += equipManager->hasFunctionCard(EXP_MAKER_25) ? 25 : 0;
	m_expRate += equipManager->hasFunctionCard(EXP_MAKER_50) ? 50 : 0;
	m_expRate += CONFIG_MANAGER->getInt("GLOBAL_EXP_RATE");

	m_donRate += equipManager->hasFunctionCard(DON_MAKER_25) ? 25 : 0;
	m_donRate += equipManager->hasFunctionCard(DON_MAKER_50) ? 50 : 0;
	m_donRate += CONFIG_MANAGER->getInt("GLOBAL_DON_RATE");

	auto weaponManager = Game::instance()->getWeaponManager();
}

RoomSessionPlayer::~RoomSessionPlayer()
{
	m_conn->decRef();
}

void RoomSessionPlayer::post(GameNetEvent* netEvent)
{
	if (m_conn == NULL)
	{
		netEvent->destroySelf(); // just in case a session player gets stuck.
		return;
	}

	try
	{
		m_conn->postNetEvent(netEvent);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void RoomSessionPlayer::initialize()
{
	m_isRespawning = false;

	m_effectManager.initialize(shared_from_this());
	m_weaponManager.initialize(shared_from_this());

	if (m_roomSession->getRoom()->isSkillsEnabled())
	{
		m_skillManager.initialize(shared_from_this());
	}

	m_entityManager.initialize(shared_from_this());
}

void RoomSessionPlayer::tick()
{
	if (canStart())
	{
		start();

		if (m_roomSession->getRoom()->isSkillsEnabled())
		{
			post(new CCUserInfo(shared_from_this()));
			getSkillManager()->resetSkillPoints();
		}

		m_roomSession->spawnPlayer(shared_from_this());
	}

	if (m_isPlaying)
	{
		m_effectManager.tick();
		m_skillManager.tick();
	}

	const auto needsToRemoveInvincibility = m_invincibleRemovalTime <= time(NULL) && m_isInvincible;

	if (needsToRemoveInvincibility)
		removeInvincibility();

	const auto needsToRespawn = m_respawnTime <= time(NULL) && m_isRespawning;

	if (needsToRespawn)
		respawn();
}

void RoomSessionPlayer::start()
{
	m_isPlaying = true;

	getPlayer()->getAchievementContainer()->resetRecent();

	m_roomSession->relayExcept<GCGameState>(getPlayer()->getId(), getPlayer()->getId(), 3);
	m_conn->postNetEvent(new GCGameState(getPlayer()->getId(), 4));

	m_roomSession->syncPlayer(shared_from_this());
}

void RoomSessionPlayer::stop()
{
	m_effectManager.clear();
	m_entityManager.close();

	const auto player = getPlayer();

	const auto curr = shared_from_this();

	player->getEquipmentManager()->finishRound(curr);
	player->getStatsManager()->apply(curr);

	Game::instance()->getLevelManager()->onPlayerFinish(curr);
	Game::instance()->getAchievementManager()->onPlayerFinish(curr);

	player->update();

	player->send(UpdateAccount(player));
}

bool RoomSessionPlayer::canStart()
{
	const auto currTime = time(NULL);

	return m_startTime <= currTime && !m_isPlaying;
}

bool RoomSessionPlayer::isInvincible()
{
	return m_isInvincible;
}

bool RoomSessionPlayer::isPlaying()
{
	return m_isPlaying;
}

void RoomSessionPlayer::togglePermanentInvincibility()
{
	if (m_isPermanentlyInvincible) {
		m_isPermanentlyInvincible = false;
		removeInvincibility();
	}
	else
	{
		m_isPermanentlyInvincible = true;
		m_isInvincible = true;
	}
}

bool RoomSessionPlayer::isPermanentlyInvincible()
{
	return m_isPermanentlyInvincible;
}

void RoomSessionPlayer::makeInvincible(uint32_t invincibleRemovalTime)
{
	m_isInvincible = true;
	m_invincibleRemovalTime = time(NULL) + invincibleRemovalTime;
}

void RoomSessionPlayer::removeInvincibility()
{
	if (m_isPermanentlyInvincible) {
		return;
	}

	m_isInvincible = false;
	m_roomSession->relayPlaying<GCGameState>(getPlayer()->getId(), 8);
}

void RoomSessionPlayer::addPlayer(Ptr player)
{
	m_conn->addSession(player);
}

uint8_t RoomSessionPlayer::getTeam()
{
	return m_team;
}

uint16_t RoomSessionPlayer::getHealth()
{
	return m_health;
}

void RoomSessionPlayer::setPosition(const Position& position)
{
	m_position = position;
}

Position RoomSessionPlayer::getPosition()
{
	return m_position;
}

void RoomSessionPlayer::addHealth(uint16_t health, bool updateClient)
{
	if (m_health > m_baseHealth + m_bonusHealth)
		return;

	if (m_health + health > m_baseHealth + m_bonusHealth)
	{
		setHealth(m_baseHealth + m_bonusHealth, updateClient);

		return;
	}

	setHealth(m_health += health, updateClient);
}

void RoomSessionPlayer::takeHealth(uint16_t health, bool updateClient)
{
	if (health > m_health)
		setHealth(0, updateClient);
	else
		setHealth(m_health - health, updateClient);
}

void RoomSessionPlayer::setHealth(uint16_t health, bool updateClient)
{
	m_health = health;

	if (updateClient) {
		post(new GCGameState(getPlayer()->getId(), 16, m_health));
	}
}

bool RoomSessionPlayer::isDead()
{
	return m_health <= 0;
}

void RoomSessionPlayer::respawn()
{
	m_isRespawning = false;
	m_roomSession->spawnPlayer(shared_from_this());
}

void RoomSessionPlayer::startRespawnCooldown(const bool hasCooldown)
{
	m_isRespawning = true;

	const auto cooldown = (hasCooldown) ? getRespawnCooldown() : 0;

	m_respawnTime = (time(nullptr) + cooldown);

	post(new GCGameState(getPlayer()->getId(), 29, cooldown * 1000));
}

void RoomSessionPlayer::setSpectating(bool isSpectating)
{
	m_isSpectating = isSpectating;
}

bool RoomSessionPlayer::isSpectating()
{
	return m_isSpectating;
}

void RoomSessionPlayer::addEventItemPickUp()
{
	m_eventItemPickUps++;
}

uint32_t RoomSessionPlayer::getEventItemPickUps()
{
	return m_eventItemPickUps;
}

uint16_t RoomSessionPlayer::getCharacter()
{
	return m_character;
}

uint16_t RoomSessionPlayer::getDefaultHealth()
{
	return m_baseHealth + m_bonusHealth;
}

uint8_t RoomSessionPlayer::getRespawnCooldown()
{
	return m_hasQuickRevive ? 5 : 7;
}

std::array<uint32_t, 9> RoomSessionPlayer::getArmor()
{
	return m_armor;
}

uint16_t RoomSessionPlayer::getExpRate()
{
	return m_expRate;
}

uint16_t RoomSessionPlayer::getDonRate()
{
	return m_donRate;
}

uint32_t RoomSessionPlayer::getDon()
{
	uint32_t don = 0;

	uint32_t playtimeDon = getPlaytime() / 8;

	if (playtimeDon >= 250)
		playtimeDon = 250;

	don += 18 * m_kills;
	don += 7 * m_deaths;
	don += 10 * m_eventItemPickUps;

	don += 20 * getTagPoints();

	don += playtimeDon;

	if (m_roomSession->getGameMode()->isMissionMode())
		don += m_score;
	float bonus = m_donRate / 100.0;
	don += don * bonus;
	return don;
}

uint16_t RoomSessionPlayer::getHighestStreak()
{
	return m_highestStreak;
}

uint16_t RoomSessionPlayer::getStreak()
{
	return m_streak;
}

void RoomSessionPlayer::resetStreak()
{
	m_streak = 0;
}

void RoomSessionPlayer::addStreak()
{
	m_streak++;

	if (m_streak > m_highestStreak)
		m_highestStreak = m_streak;
}

void RoomSessionPlayer::healTeam(uint16_t healing)
{
	auto players = m_roomSession->getPlayersForTeam(m_team);

	for (auto& player : players)
	{
		if (player->isDead())
			continue;

		player->addHealth(50, true);

		if (player->getPlayer()->getId() != getPlayer()->getId())
			player->post(new GCGameItem(1, player->getPlayer()->getId(), 1191182350, NULL));
	}
}

uint16_t RoomSessionPlayer::getScore()
{
	return m_score;
}

uint16_t RoomSessionPlayer::getKills()
{
	return m_kills;
}

uint16_t RoomSessionPlayer::getDeaths()
{
	return m_deaths;
}

uint32_t RoomSessionPlayer::getPlaytime()
{
	const auto currTime = time(NULL);

	if (currTime < m_startTime)
		return 0;

	return currTime - m_startTime;
}

void RoomSessionPlayer::setHighestMultiKill(uint32_t count)
{
	m_highestMultiKill = count;
}

uint32_t RoomSessionPlayer::getHighestMultiKill()
{
	return m_highestMultiKill;
}

uint32_t RoomSessionPlayer::getExperience()
{
	uint32_t experience = 0;
	uint32_t playTimeExperience = getPlaytime() / 4;

	if (playTimeExperience >= 500)
		playTimeExperience = 500;

	experience += 35 * m_kills;
	experience += 10 * m_deaths;
	experience += 10 * m_eventItemPickUps;

	experience += 25 * getTagPoints();

	experience += playTimeExperience;

	if (m_roomSession->getGameMode()->isMissionMode())
		experience += m_score;
	float bonus = m_expRate / 100.0;
	experience += experience * bonus;

	return experience;
}

void RoomSessionPlayer::addKill()
{
	m_kills++;
}

void RoomSessionPlayer::addDeath()
{
	m_deaths++;
}

void RoomSessionPlayer::addScore(uint16_t score)
{
	m_score += score;

	m_roomSession->addPointsForTeam(m_team, score);
}

uint16_t RoomSessionPlayer::getTagKillsAsPlayer()
{
	return m_tagKillsAsPlayer;
}

void RoomSessionPlayer::addTagKillAsPlayer()
{
	m_tagKillsAsPlayer += 1;
}

uint16_t RoomSessionPlayer::getPlayerKillsAsTag()
{
	return m_playerKillsAsTag;
}

void RoomSessionPlayer::addPlayerKillAsTag()
{
	m_playerKillsAsTag += 1;
}

uint16_t RoomSessionPlayer::getDeathsAsTag()
{
	return m_deathsAsTag;
}

void RoomSessionPlayer::addDeathAsTag()
{
	m_deathsAsTag += 1;
}

uint16_t RoomSessionPlayer::getDeathsByTag()
{
	return m_deathsByTag;
}

void RoomSessionPlayer::addDeathByTag()
{
	m_deathsByTag += 1;
}

uint16_t RoomSessionPlayer::getTagPoints()
{
	return ((m_tagKillsAsPlayer * 5) + (m_playerKillsAsTag * 1));
}

uint32_t RoomSessionPlayer::getTimeAliveAsTag()
{
	return m_timeAliveAsTag;
}

void RoomSessionPlayer::addTimeAliveAsTag(uint32_t time)
{
	m_timeAliveAsTag += time;
}

uint32_t RoomSessionPlayer::getDamageDealtToTag()
{
	return m_damageDealtToTag;
}

void RoomSessionPlayer::addDamageDealtToTag(uint32_t damage)
{
	m_damageDealtToTag += damage;
}

uint32_t RoomSessionPlayer::getDamageDealtAsTag()
{
	return m_damageDealtAsTag;
}

void RoomSessionPlayer::addDamageDealtAsTag(uint32_t damage)
{
	m_damageDealtAsTag += damage;
}

std::shared_ptr<Player> RoomSessionPlayer::getPlayer()
{
	return m_conn->getPlayer();
}

GameConnection* RoomSessionPlayer::getGameConnection()
{
	return m_conn;
}

std::shared_ptr<RoomSession> RoomSessionPlayer::getRoomSession()
{
	return m_roomSession;
}

PlayerEffectManager* RoomSessionPlayer::getEffectManager()
{
	return &m_effectManager;
}

PlayerWeaponManager* RoomSessionPlayer::getWeaponManager()
{
	return &m_weaponManager;
}

PlayerSkillManager* RoomSessionPlayer::getSkillManager()
{
	return &m_skillManager;
}

PlayerEntityManager* RoomSessionPlayer::getEntityManager()
{
	return &m_entityManager;
}
