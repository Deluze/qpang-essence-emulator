#include "GameMode.h"

#include <assert.h>


#include "StringConverter.h"
#include "qpang/Game.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

bool GameMode::isTeamMode()
{
	return false;
}

bool GameMode::isMissionMode()
{
	return false;
}

void GameMode::tick(std::shared_ptr<RoomSession> roomSession)
{
}

void GameMode::onApply(std::shared_ptr<Room> room)
{
	room->balancePlayers();
}

void GameMode::onStart(std::shared_ptr<RoomSession> roomSession)
{
}

void GameMode::onPlayerSync(std::shared_ptr<RoomSessionPlayer> session)
{
}

void GameMode::onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation)
{
	assert(killer != nullptr);
	assert(target != nullptr);

	auto roomSession = killer->getRoomSession();
	const auto isTeamMode = killer->getRoomSession()->getGameMode()->isTeamMode();
	const auto isSuicide = killer == target;
	const auto isSameTeam = killer->getTeam() == target->getTeam();

	if (isSuicide)
	{
		killer->addDeath();
	}
	else
	{
		const auto earnedCash = rand() % 20 == 0;

		if (earnedCash)
		{
			auto player = killer->getPlayer();

			const auto cashCount = (rand() % 10) + 5;
			const auto targetName = StringConverter::Utf16ToUtf8(target->getPlayer()->getName());

			char buffer[70];
			sprintf_s(buffer, "You earned %d cash by killing %s!", cashCount, targetName.c_str());
			player->addCash(cashCount);
			player->broadcast(StringConverter::Utf8ToUtf16(buffer));


			// give the player that died some love ;(

			auto targetPlayer = target->getPlayer();

			const auto cash = (rand() % 5) + 1;
			const auto name = StringConverter::Utf16ToUtf8(player->getName());

			char buffer2[90];
			sprintf_s(buffer2, "You earned %d cash because %s got some too!", cash, name.c_str());
			targetPlayer->addCash(cash);
			targetPlayer->broadcast(StringConverter::Utf8ToUtf16(buffer2));
		}

		const auto earnedCoin = rand() % 150 == 0;

		if (earnedCoin)
		{
			killer->getPlayer()->addCoins(100);
			killer->getPlayer()->broadcast(u"Lucky! You just found a golden coin!");
		}
		
		killer->addKill();
		target->addDeath();

		if (!isMissionMode())
		{
			if (!isSameTeam && isTeamMode)
				killer->addScore();
			else if (!isTeamMode)
				killer->addScore();
		}

		killer->addStreak();
	}

	target->resetStreak();
	target->getEffectManager()->clear();
	target->startRespawnCooldown();

	auto* killerStats = killer->getPlayer()->getStatsManager();
	auto* targetStats = target->getPlayer()->getStatsManager();

	if (hitLocation == 0)
	{
		killerStats->addHeadshotKills();
		targetStats->addHeadshotDeaths();
	}

	if (killer->getTeam() == target->getTeam() && isTeamMode)
	{
		killerStats->addTeamKills();
		targetStats->addTeamDeaths();
	}

	switch (weapon.weaponType)
	{
	case WeaponType::MELEE:
		killerStats->addMeleeKills();
		break;
	case WeaponType::RIFLE:
		killerStats->addGunKills();
		break;
	case WeaponType::LAUNCHER:
		killerStats->addLauncherKills();
		break;
	case WeaponType::BOMB:
	default:
		killerStats->addBombKills();
		break;
	}

	if (roomSession->canFinish())
		roomSession->finish();
}