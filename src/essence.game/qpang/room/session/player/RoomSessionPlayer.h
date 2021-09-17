#pragma once

#include <memory>
#include <mutex>
#include <cstdint>

#include "qpang/Position.h"
#include "qpang/room/session/bullet/Bullet.h"

#include "qpang/room/session/player/effect/PlayerEffectManager.h"
#include "qpang/room/session/player/weapon/PlayerWeaponManager.h"
#include "qpang/room/session/player/skill/PlayerSkillManager.h"
#include "qpang/room/session/player/entity/PlayerEntityManager.h"

class GameNetEvent;
class RoomSession;
class GameConnection;
class Player;
class Skill;

class RoomSessionPlayer : public std::enable_shared_from_this<RoomSessionPlayer>
{
public:
	using Ptr = std::shared_ptr<RoomSessionPlayer>;

	RoomSessionPlayer(GameConnection* conn, std::shared_ptr<RoomSession> roomSession, uint8_t team);
	~RoomSessionPlayer();

	void post(GameNetEvent* netEvent);

	void initialize();

	void tick();
	void start();
	void stop();

	bool canStart();
	bool isInvincible();
	bool isPermanentlyInvincible();
	bool isPlaying();

	void togglePermanentInvincibility();

	void makeInvincible();
	void removeInvincibility();
	void addPlayer(RoomSessionPlayer::Ptr player);

	uint8_t getTeam();
	uint16_t getHealth();

	void setPosition(const Position& position);
	Position getPosition();

	void addHealth(uint16_t health, bool updateOnClient = false);
	void takeHealth(uint16_t health, bool updateOnClient = false);
	void setHealth(uint16_t health, bool updateOnClient = false);
	bool isDead();

	void respawn();
	void startRespawnCooldown();

	void setSpectating(bool isSpectating = true);
	bool isSpectating();

	void addEventItemPickUp();
	uint32_t getEventItemPickUps();

	uint16_t getCharacter();
	uint16_t getDefaultHealth();
	uint8_t getRespawnCooldown();

	std::array<uint32_t, 9> getArmor();

	uint16_t getExpRate();
	uint16_t getDonRate();

	uint32_t getExperience();
	uint32_t getDon();

	uint16_t getHighestStreak();
	uint16_t getStreak();
	void resetStreak();
	void addStreak();
	void healTeam(uint16_t healing);

	uint16_t getScore();
	uint16_t getPublicEnemyScore();

	uint16_t getKills();
	uint16_t getDeaths();
	uint32_t getPlaytime();

	void setHighestMultiKill(uint32_t count);
	uint32_t getHighestMultiKill();

	void addKill();
	void addDeath();
	void addPublicEnemyScore(uint16_t publicEnemyScore);
	void addScore(uint16_t score = 1);

	std::shared_ptr<Player> getPlayer();
	GameConnection* getGameConnection();
	std::shared_ptr<RoomSession> getRoomSession();
	PlayerEffectManager* getEffectManager();
	PlayerWeaponManager* getWeaponManager();
	PlayerSkillManager* getSkillManager();
	PlayerEntityManager* getEntityManager();

	template<typename T, typename ...Args>
	void send(Args... args)
	{
		auto evt = new T(args...);

		post(evt);
	}
private:

	PlayerEffectManager m_effectManager;
	PlayerWeaponManager m_weaponManager;
	PlayerSkillManager m_skillManager;
	PlayerEntityManager m_entityManager;

	Position m_position;

	time_t m_joinTime;
	time_t m_startTime;
	time_t m_invincibleRemovalTime;
	time_t m_respawnTime;

	bool m_isPlaying; // true if waiting for players is over
	bool m_isRespawning;

	bool m_isSpectating;

	uint16_t m_character = 343;
	uint8_t m_team = 0;
	uint16_t m_health = 100;
	uint16_t m_baseHealth = 100;
	uint16_t m_bonusHealth = 0;

	std::array<uint32_t, 9> m_armor;

	std::mutex m_bulletMx;
	
	bool m_hasQuickRevive;
	bool m_isInvincible;
	bool m_isPermanentlyInvincible;

	uint16_t m_expRate;
	uint16_t m_donRate;

	uint32_t m_exp;
	uint32_t m_don;

	uint16_t m_highestStreak;
	uint16_t m_streak;
	uint16_t m_kills;
	uint16_t m_deaths;
	uint16_t m_score;
	uint16_t m_publicEnemyScore;
	uint32_t m_playTime;
	uint32_t m_highestMultiKill;
	uint32_t m_eventItemPickUps;
	
	GameConnection* m_conn;
	std::shared_ptr<RoomSession> m_roomSession;
};