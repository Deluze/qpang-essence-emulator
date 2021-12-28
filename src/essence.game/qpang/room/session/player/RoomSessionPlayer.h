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
	void stopPveGame();

	bool canStart();
	bool isInvincible();
	bool isPermanentlyInvincible();
	bool isPlaying();

	void togglePermanentInvincibility();

	void makeInvincible(uint32_t invincibleRemovalTime = 5);
	void removeInvincibility();
	void addPlayer(RoomSessionPlayer::Ptr player);

	uint8_t getTeam();
	uint16_t getHealth();

	void setPosition(const Position& position);
	Position getPosition();

	void setFloorNumber(const uint8_t floorNumber);
	uint8_t getFloorNumber();

	void addHealth(uint16_t health, bool updateOnClient = false);
	void takeHealth(uint16_t health, bool updateOnClient = false);
	void setHealth(uint16_t health, bool updateOnClient = false);

	void setPermanentlyDead(bool permanentlyDead);

	bool isDead();
	bool isPermanentlyDead();

	bool canRespawn();
	void setCanRespawn(bool canRespawn);

	void respawn();
	void startRespawnCooldown(bool hasCooldown = true);

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

	uint16_t getKills();
	uint16_t getDeaths();
	uint32_t getPlaytime();

	void setHighestMultiKill(uint32_t count);
	uint32_t getHighestMultiKill();

	void addKill();
	void addDeath();
	void addScore(uint16_t score = 1);

	uint16_t getTagKillsAsPlayer();
	void addTagKillAsPlayer();

	uint16_t getPlayerKillsAsTag();
	void addPlayerKillAsTag();

	uint16_t getDeathsAsTag();
	void addDeathAsTag();

	uint16_t getDeathsByTag();
	void addDeathByTag();

	uint16_t getTagPoints();

	uint32_t getTimeAliveAsTag();
	void addTimeAliveAsTag(uint32_t time = 1);

	uint32_t getDamageDealtToTag();
	void addDamageDealtToTag(uint32_t damage);

	uint32_t getDamageDealtAsTag();
	void addDamageDealtAsTag(uint32_t damage);

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

	bool m_floorNumber;

	time_t m_joinTime;
	time_t m_startTime;
	time_t m_invincibleRemovalTime;
	time_t m_respawnTime;

	bool m_isPlaying; // true if waiting for players is over
	bool m_permanentlyDead;

	bool m_canRespawn;
	bool m_isRespawning;

	bool m_isSpectating;

	uint16_t m_character = 343;
	uint8_t m_team = 0;
	uint16_t m_health = 100;
	uint16_t m_baseHealth = 100;
	uint16_t m_bonusHealth = 0;

	std::array<uint32_t, 9> m_armor;

	std::mutex m_bulletMx;

	uint8_t m_respawnCooldown;
	
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
	uint32_t m_playTime;
	uint32_t m_highestMultiKill;
	uint32_t m_eventItemPickUps;

	uint16_t m_deathsAsTag;
	uint16_t m_deathsByTag;

	uint16_t m_tagKillsAsPlayer;
	uint16_t m_playerKillsAsTag;

	uint32_t m_timeAliveAsTag;
	uint32_t m_damageDealtToTag;
	uint32_t m_damageDealtAsTag;
	
	GameConnection* m_conn;
	std::shared_ptr<RoomSession> m_roomSession;
};