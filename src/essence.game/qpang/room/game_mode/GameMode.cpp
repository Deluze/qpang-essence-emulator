#include "GameMode.h"

#include <assert.h>


#include "StringConverter.h"
#include "qpang/Game.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"
#include "qpang/room/tnl/net_events/client/cg_weapon.hpp"
#include "qpang/room/tnl/net_events/server/gc_weapon.hpp"
#include "qpang/room/tnl/net_events/server/gc_card.hpp"

constexpr auto MACHINE_GUN_ITEM_ID = 1095303174;

bool GameMode::isTeamMode()
{
	return false;
}

bool GameMode::isMissionMode()
{
	return false;
}

bool GameMode::isPublicEnemyMode()
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
	const auto areSkillsEnabled = session->getRoomSession()->getRoom()->isSkillsEnabled();

	for (const auto &playingPlayer : session->getRoomSession()->getPlayingPlayers())
	{
		const auto playingPlayerHasEquippedMachineGun = playingPlayer->getWeaponManager()->hasEquippedMachineGun();

		if (playingPlayerHasEquippedMachineGun)
		{
			const auto seqId = playingPlayer->getWeaponManager()->getEquippedMachineGunSeqId();

			session->post(new GCWeapon(playingPlayer->getPlayer()->getId(), CGWeapon::CMD::EQUIP_MACHINE_GUN, MACHINE_GUN_ITEM_ID, seqId));
		}

		const auto playingPlayerHasActiveSkillCard = playingPlayer->getSkillManager()->hasActiveSkillCard();

		if (areSkillsEnabled && playingPlayerHasActiveSkillCard)
		{
			const auto activeSkillCard = playingPlayer->getSkillManager()->getActiveSkillCard();

			const auto targetPlayerId = playingPlayer->getSkillManager()->getActiveSkillCardTargetPlayerId();

			const auto itemId = activeSkillCard->getItemId();
			const auto seqId = playingPlayer->getSkillManager()->getActiveSkillCardTargetPlayerId();

			session->post(new GCCard(playingPlayer->getPlayer()->getId(), targetPlayerId, CGCard::CMD::CARD_BEGIN, CGCard::CardType::SKILL_CARD, itemId, seqId));
		}
	}
}

void GameMode::onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation)
{
	assert(killer != nullptr);
	assert(target != nullptr);

	const auto roomSession = killer->getRoomSession();

	const auto isTeamMode = roomSession->getGameMode()->isTeamMode();

	const auto isSuicide = (killer == target);
	const auto isSameTeam = (killer->getTeam() == target->getTeam());

	const auto isPublicEnemyMode = roomSession->getGameMode()->isPublicEnemyMode();

	const auto areSkillCardsEnabled = roomSession->getRoom()->isSkillsEnabled();

	const auto targetHasActiveSkillCard = target->getSkillManager()->hasActiveSkillCard();
	const auto killerHasActiveSkillCard = killer->getSkillManager()->hasActiveSkillCard();

	const auto targetPlayerHasRainbowSkillCard = areSkillCardsEnabled && targetHasActiveSkillCard && target->getSkillManager()->getActiveSkillCard()->isRainbowSkillCard();
	const auto killerPlayerHasRainbowSkillCard = areSkillCardsEnabled && killerHasActiveSkillCard && killer->getSkillManager()->getActiveSkillCard()->isRainbowSkillCard();

	if (isSuicide && !isPublicEnemyMode)
	{
		killer->addDeath();
	}
	else if (!isPublicEnemyMode)
	{
		const auto earnedCash = rand() % 20 == 0; // 20 % kans

		if (earnedCash)
		{
			auto player = killer->getPlayer();

			const auto cashCount = (rand() % 10) + 5; // maximaal 15 cash, minimaal 5
			const auto targetName = StringConverter::Utf16ToUtf8(target->getPlayer()->getName());

			char buffer[80];
			sprintf_s(buffer, "You have earned %d cash by killing %s!", cashCount, targetName.c_str());

			player->addCash(cashCount);
			player->broadcast(StringConverter::Utf8ToUtf16(buffer));


			// give the player that died some love ;(

			auto targetPlayer = target->getPlayer();

			const auto cash = (rand() % 5) + 1; // minimaal 1 maximaal 6.
			const auto name = StringConverter::Utf16ToUtf8(player->getName());

			char buffer2[120];
			sprintf_s(buffer2, "You have earned %d cash because %s got some too!", cash, name.c_str());
			targetPlayer->addCash(cash);
			targetPlayer->broadcast(StringConverter::Utf8ToUtf16(buffer2));
		}

		const auto killedHasEarnedCoin = rand() % 150 == 0;
		const auto targetHasEarnedCoin = rand() % 300 == 0;

		if (killedHasEarnedCoin)
		{
			killer->getPlayer()->addCoins(100);
			killer->getPlayer()->broadcast(u"It's your lucky today. You have just found a golden coin!");
		}

		if (targetHasEarnedCoin) {
			target->getPlayer()->addCoins(100);
			target->getPlayer()->broadcast(u"It's your lucky day. You died, fell down on the ground and found a golden coin!");
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
	}

	if (!isSuicide)
	{
		killer->addStreak();
		
		if (areSkillCardsEnabled)
		{
			if (!killerPlayerHasRainbowSkillCard)
			{
				const auto skillPoints = 45;
				const auto skillGaugeBoostPercentage = killer->getSkillManager()->getSkillGaugeBoostPercentage();

				killer->getSkillManager()->addSkillPoints((uint32_t) (skillPoints + (skillPoints * skillGaugeBoostPercentage)));
			}
		}
	}

	target->resetStreak();
	target->getEffectManager()->clear();
	target->startRespawnCooldown();

	auto* killerStats = killer->getPlayer()->getStatsManager();
	auto* targetStats = target->getPlayer()->getStatsManager();

	const auto isHeadshot = (hitLocation == 0);

	if (isHeadshot && !isPublicEnemyMode)
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