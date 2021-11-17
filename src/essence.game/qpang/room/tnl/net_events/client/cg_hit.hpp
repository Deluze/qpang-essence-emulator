#ifndef CG_HIT_H
#define CG_HIT_H

#include "AABBHelper.h"
#include "GameNetEvent.h"
#include "qpang/Game.h"
#include "qpang/room/tnl/net_events/server/gc_hit.hpp"

// ReSharper disable once CppInconsistentNaming
class CGHit final : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	enum HitLocation
	{
		FRONT = 1,
		BACK = 2,
		BACK_SIDE = 3
	};

	enum BodyPartHitLocation : U8 {
		HEAD = 0,
		BODY = 1,
		RIGHT_ARM = 2,
		RIGHT_HAND = 3,
		LEFT_ARM = 4,
		LEFT_HAND = 5,
		RIGHT_LEG = 6,
		RIGHT_FEET = 7,
		LEFT_LEG = 8,
		LEFT_FEET = 9,
		SPLASH_1 = 10,
		SPLASH_2 = 11
		// Explosion? Trowable? = 11
	};

	enum HitType : U8
	{
		PLAYER_HIT = 0,
		PLAYER_NO_HIT = 1,
		FALL_DAMAGE = 2
	};

	enum MapObject : U8 {
		TRAP_FLAME = 1,
		TRAP_PRESS = 2,
		TRAP_FALL = 3,
		TRAP_DOWN = 4,
		TRAP_POISON_GROUND = 6,
		TRAP_SHIP_PROPELLER = 7
	};

	CGHit() : GameNetEvent{ CG_HIT, Guaranteed, DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) override {};
	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&srcPlayerId);
		bstream->read(&dstPlayerId);
		bstream->read(&unk_03);
		bstream->read(&srcPosX);
		bstream->read(&srcPosY);
		bstream->read(&srcPosZ);
		bstream->read(&dstPosX);
		bstream->read(&dstPosY);
		bstream->read(&dstPosZ);
		bstream->read(&entityId);
		bstream->read(&hitType);
		bstream->read(&bodyPartHitLocation);
		bstream->read(&weaponId);
		bstream->read(&rtt);
		bstream->read(&weaponType);
		bstream->read(&hitLocation);
		bstream->read(&unk_17);
		bstream->read(&unk_18);

		//std::cout << "================================" << std::endl;
		//std::cout << "CGHit::unpack >> SourcePlayerId: " << (int)srcPlayerId << ", TargetPlayerId: " << (int)dstPlayerId << ", EntityId: " << (int)entityId << std::endl;
		//std::cout << "CGHit::unpack >> WeaponId: " << (int)weaponId << ", WeaponType: " << (int)weaponType << std::endl;
		//std::cout << "CGHit::unpack >> BodyPart: " << (int)bodyPartHitLocation << ", HitLocation: " << (int)hitLocation << ", HitType: " << (int)hitType << std::endl;
		//std::cout << "CGHit::unpack >> Unk03: " << (int)unk_03 << ", Unk17: " << (int)unk_17 << ", Unk18: " << (int)unk_18 << ", RTT: " << (int)rtt << std::endl;
		//std::cout << "================================" << std::endl;
	};

	// TODO: Rewrite this code, rewrite damage system, figure out how to "detect" special attack.
	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		//std::cout << "CGHit::handle >> Player " << srcPlayerId << " hit player " << dstPlayerId
		//	<< ". Unk03: " << (int)unk_03 << ", Unk16: " << (int)hitLocation << ", Unk17: "
		//	<< (int)unk_17 << ", Unk18: " << (int)unk_18 << "." << std::endl;

		if (const auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (roomPlayer->isSpectating())
			{
				return;
			}

			if (const auto session = roomPlayer->getRoomSessionPlayer(); session != nullptr)
			{
				if (!session->getWeaponManager()->hasWeapon(weaponId) && !isTrap(weaponId))
				{
					return;
				}

				if (dstPlayerId == NULL)
				{
					hitEmpty(weaponId, session);
				}
				else if (srcPlayerId == dstPlayerId)
				{
					hit(weaponId, session, session, hitType);
				}
				else
				{
					const auto targetSession = session->getRoomSession()->find(dstPlayerId);

					if (targetSession == nullptr)
					{
						return;
					}

					if (targetSession->isInvincible() || targetSession->isDead())
					{
						return;
					}

					hit(weaponId, session, targetSession, hitType);
				}
			}
		}
	}

	void hit(const uint32_t weaponId, const RoomSessionPlayer::Ptr& srcPlayer, const RoomSessionPlayer::Ptr& dstPlayer, uint8_t bodyPart) const
	{
		// TODO: Rework this shit cus its very much unreadable code.
		if (dstPlayer->isDead())
		{
			return;
		}

		uint16_t damage;
		uint8_t effectId = 0;

		const auto roomSession = srcPlayer->getRoomSession();
		const auto areSkillsEnabled = roomSession->getRoom()->isSkillsEnabled();

		auto isTeamMode = roomSession->getGameMode()->isTeamMode();
		auto isSameTeam = (isTeamMode && (srcPlayer->getTeam() == dstPlayer->getTeam()));

		auto weaponUsed = Game::instance()->getWeaponManager()->get(weaponId);

		if (weaponUsed.weaponType != WeaponType::BOMB && srcPlayer->isDead())
		{
			return;
		}

		const auto isTrapWeapon = isTrap(weaponId);

		if (isTrapWeapon)
		{
			damage = getTrapDamage(weaponId);

			if (weaponId == TRAP_FALL)
			{
				if (dstPlayer == roomSession->getEssenceHolder())
				{
					roomSession->resetEssence();
				}
			}

			dstPlayer->takeHealth(damage);
		}
		else
		{
			constexpr auto octoMineItemId = 1095434246;

			if (weaponId != octoMineItemId) // Jump mine
			{
				if (const auto isValidShot = srcPlayer->getEntityManager()->isValidShot(entityId); !isValidShot)
				{
					return;
				}
			}

			damage = weaponUsed.damage;

			switch (bodyPartHitLocation)
			{
			case HEAD:
				damage *= 1;
				break;
			case BODY:
				damage *= 0.9;
				break;
			case LEFT_LEG:
			case RIGHT_LEG:
			case LEFT_ARM:
			case RIGHT_ARM:
				damage *= 0.8;
				break;
			case LEFT_FEET:
			case RIGHT_FEET:
			case LEFT_HAND:
			case RIGHT_HAND:
				damage *= 0.6;
				break;
			}

			const auto dstPlayerHasActiveSkill = dstPlayer->getSkillManager()->hasActiveSkill();

			if (areSkillsEnabled)
			{
				const auto dstPlayerShouldReceiveReducedDamageFromAllSources = dstPlayerHasActiveSkill
					&& dstPlayer->getSkillManager()->getActiveSkill()->isStuntUpSkill();

				const auto dstPlayerShouldReceiveReducedDamageFromLaunchers = dstPlayerHasActiveSkill
					&& dstPlayer->getSkillManager()->getActiveSkill()->isMentalUpSkill();

				const auto weaponUsedIsLauncher = (weaponUsed.weaponType == WeaponType::LAUNCHER);

				if (dstPlayerShouldReceiveReducedDamageFromAllSources)
				{
					// 25% reduced damage.
					damage *= 0.75;
				}
				else if (dstPlayerShouldReceiveReducedDamageFromLaunchers && weaponUsedIsLauncher)
				{
					damage += 0.50;
				}
			}

			const auto isPublicEnemyMode = roomSession->getGameMode()->isPublicEnemyMode();

			const auto srcPlayerIsTagged = (isPublicEnemyMode) && (roomSession->getCurrentlySelectedTag() == srcPlayer->getPlayer()->getId());
			const auto dstPlayerIsTagged = (isPublicEnemyMode) && (roomSession->getCurrentlySelectedTag() == dstPlayer->getPlayer()->getId());

			if (isPublicEnemyMode)
			{
				if (srcPlayerIsTagged && !dstPlayerIsTagged) // Shooter is tag, target is player.
				{
					isSameTeam = false;
				}
				else if (!srcPlayerIsTagged && dstPlayerIsTagged)
				{
					isSameTeam = false;
				}
				else if (!srcPlayerIsTagged && !dstPlayerIsTagged)
				{
					isSameTeam = true;
				}
			}

			if (isSameTeam)
			{
				damage = 0; // still apply debuffs
			}

			if (weaponId == octoMineItemId)
			{
				if (srcPlayer == dstPlayer)
				{
					damage = 0;
				}
			}

			if (srcPlayerIsTagged)
			{
				srcPlayer->addDamageDealtAsTag(damage);
			}

			if (dstPlayerIsTagged)
			{
				srcPlayer->addDamageDealtToTag(damage);
			}

			if (areSkillsEnabled && srcPlayer->getSkillManager()->hasActiveSkill())
			{
				if (const auto srcPlayerShouldInstantlyKillEnemyWithMeleeWeapon = srcPlayer->getSkillManager()->getActiveSkill()->isAssassinSkill();
					srcPlayerShouldInstantlyKillEnemyWithMeleeWeapon && (weaponUsed.weaponType == WeaponType::MELEE) && !isSameTeam)
				{
					damage = 9999;

					srcPlayer->getSkillManager()->deactivateSkill();
				}
			}

			const auto dstPlayerShouldIgnoreDamageFromAllSources = dstPlayerHasActiveSkill &&
				dstPlayer->getSkillManager()->getActiveSkill()->IsIronWallSkill();

			if (dstPlayerShouldIgnoreDamageFromAllSources && !isSameTeam)
			{
				damage = 1;
			}

			if (areSkillsEnabled && dstPlayerHasActiveSkill && !isSameTeam)
			{
				const auto activeSkill = dstPlayer->getSkillManager()->getActiveSkill();
				const auto dstPlayerShouldOnlyTakeMeleeDamage = activeSkill->shouldOnlyTakeMeleeDamage();

				if (dstPlayerShouldOnlyTakeMeleeDamage && weaponUsed.weaponType != WeaponType::MELEE)
				{
					damage = 0;
				}
			}

			const auto srcPlayerHasActiveSkill = areSkillsEnabled && srcPlayer->getSkillManager()->hasActiveSkill();

			if (areSkillsEnabled && srcPlayerHasActiveSkill && !isSameTeam)
			{
				const auto activeSkill = srcPlayer->getSkillManager()->getActiveSkill();
				const auto srcPlayerShouldOnlyDealMeleeDamage = activeSkill->shouldOnlyDealMeleeDamage();

				if (srcPlayerShouldOnlyDealMeleeDamage && weaponUsed.weaponType != WeaponType::MELEE)
				{
					damage = 0;
				}
			}

			if (areSkillsEnabled && dstPlayerHasActiveSkill && !isSameTeam)
			{
				const auto activeSkill = dstPlayer->getSkillManager()->getActiveSkill();
				const auto isEnergyShieldSkill = activeSkill->isEnergyShieldSkill();

				if (isEnergyShieldSkill)
				{
					const auto isSplashDamage = (bodyPartHitLocation == SPLASH_1 || bodyPartHitLocation == SPLASH_2);
					const auto isBackDamage = (hitLocation != HitLocation::FRONT);
					const auto isCloseRange = AABBHelper::getDistanceBetweenPositions(srcPlayer->getPosition(), dstPlayer->getPosition()) <= 3;

					if (!isCloseRange && !isBackDamage && !isSplashDamage)
					{
						damage = 0;
					}
				}
			}

			dstPlayer->takeHealth(damage);

			if (const auto applyEffect = (rand() % 100) <= weaponUsed.effectChance;
				applyEffect && !dstPlayerShouldIgnoreDamageFromAllSources && !isSameTeam)
			{
				if (dstPlayerIsTagged && weaponUsed.effectDuration > 2)
				{
					weaponUsed.effectDuration = 2;
				}

				effectId = weaponUsed.effectId;
				dstPlayer->getEffectManager()->addEffect(srcPlayer, weaponUsed, entityId);
			}
		}

		const auto srcId = srcPlayer->getPlayer()->getId();
		const auto dstId = dstPlayer->getPlayer()->getId();

		// Why was it previously so that GCHit was never relayed to everyone else if the src and dst player were the same? Certainly explains why octomines didn't disappear.
		roomSession->relayPlaying<GCHit>(srcPlayerId, dstPlayerId, unk_03, srcPosX, srcPosY, srcPosZ, dstPosX, dstPosY, dstPosZ, entityId,
			hitType, bodyPartHitLocation, dstPlayer->getHealth(), damage, weaponId, rtt, weaponType, hitLocation, srcPlayer->getStreak() + 1, unk_18, effectId);

		if (areSkillsEnabled && !isSameTeam)
		{
			const auto dstPlayerHasActiveSkill = dstPlayer->getSkillManager()->hasActiveSkill();
			const auto dstPlayerHasRainbowSkillCard = dstPlayerHasActiveSkill
				&& dstPlayer->getSkillManager()->getActiveSkill()->getSkillRateType() == SkillRateType::RAINBOW;
			const auto dstPlayerShouldIgnoreDamageFromAllSources = dstPlayerHasActiveSkill
				&& dstPlayer->getSkillManager()->getActiveSkill()->IsIronWallSkill();

			const auto isValidWeapon = !isTrapWeapon
				&& ((weaponUsed.weaponType == WeaponType::MELEE) || (weaponUsed.weaponType == WeaponType::RIFLE) || (weaponUsed.weaponType == WeaponType::LAUNCHER));

			if (!dstPlayerHasRainbowSkillCard && !dstPlayerShouldIgnoreDamageFromAllSources && isValidWeapon)
			{
				const auto skillPointsToAdd = (bodyPartHitLocation == HEAD) ? 10 : 5;
				const auto skillGaugeBoostPercentage = dstPlayer->getSkillManager()->getSkillGaugeBoostPercentage();

				dstPlayer->getSkillManager()->addSkillPoints(static_cast<uint32_t>(skillPointsToAdd + (skillPointsToAdd * skillGaugeBoostPercentage)));
			}

			if (dstPlayer->getSkillManager()->hasActiveSkill())
			{
				if (dstPlayer->getSkillManager()->getActiveSkill()->shouldDisableWhenDamageIsTaken())
				{
					dstPlayer->getSkillManager()->deactivateSkill();
				}
			}
		}

		if (dstPlayer->isDead())
		{
			if (dstPlayer->getWeaponManager()->hasEquippedMachineGun())
			{
				dstPlayer->getWeaponManager()->unequipMachineGun();
			}

			srcPlayer->getEntityManager()->addKill(entityId);

			// Killfeed?
			roomSession->relayPlaying<GCGameState>(dstId, bodyPartHitLocation == 0 ? 28 : 17, weaponId, srcId);
			roomSession->getGameMode()->onPlayerKill(srcPlayer, dstPlayer, weaponUsed, bodyPartHitLocation);
		}
	}

	void hitEmpty(const uint32_t weaponId, const RoomSessionPlayer::Ptr& srcPlayer) const
	{
		const auto roomSession = srcPlayer->getRoomSession();

		roomSession->relayPlaying<GCHit>(srcPlayerId, 0, unk_03, srcPosX, srcPosY, srcPosZ, dstPosX, dstPosY, dstPosZ, entityId,
			hitType, bodyPartHitLocation, 0, 0, weaponId, rtt, weaponType, hitLocation, srcPlayer->getStreak() + 1, unk_18, 0);
	}

	static bool isTrap(const uint32_t weaponId)
	{
		return weaponId == TRAP_FLAME
			|| weaponId == TRAP_POISON_GROUND
			|| weaponId == TRAP_SHIP_PROPELLER
			|| weaponId == TRAP_PRESS
			|| weaponId == TRAP_FALL
			|| weaponId == TRAP_DOWN;
	}

	[[nodiscard]] uint16_t getTrapDamage(uint32_t trapId) const
	{
		switch (weaponId)
		{
		case TRAP_FLAME:
		case TRAP_POISON_GROUND:
		case TRAP_SHIP_PROPELLER:
			return 10;
		case TRAP_PRESS:
			return 150;
		default:
		case TRAP_FALL:
			return 9999;
		case TRAP_DOWN: // fall damage
			return 0;
		}
	}

	void process(EventConnection* ps) override
	{
		post<CGHit>(ps);
	};

	U32 srcPlayerId; //140
	U32 dstPlayerId; //144
	U32 unk_03; //88
	F32 srcPosX; //116
	F32 srcPosY; //120
	F32 srcPosZ; //124
	F32 dstPosX; //128
	F32 dstPosY; //132
	F32 dstPosZ; //136
	U32 entityId; //92
	U8 hitType; //148 
	U8 bodyPartHitLocation; //149

	U32 weaponId; //96
	U64 rtt; //104
	U8 weaponType; //112
	U8 hitLocation; //150
	U8 unk_17; //151
	U32 unk_18; //152

	TNL_DECLARE_CLASS(CGHit);
};
#endif