#ifndef CG_HIT_H
#define CG_HIT_H

#include "GameNetEvent.h"
#include "qpang/Game.h"
#include "qpang/room/tnl/net_events/server/gc_card.hpp"
#include "qpang/room/tnl/net_events/server/gc_hit.hpp"

// ReSharper disable once CppInconsistentNaming
class CGHit final : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	enum HitLocation : U8 {
		HEAD = 0,
		BODY = 1,
		R_ARM = 2,
		R_HAND = 3,
		L_ARM = 4,
		L_HAND = 5,
		R_LEG = 6,
		R_FEET = 7,
		L_LEG = 8,
		L_FEET = 9
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
		bstream->read(&hitLocation);
		bstream->read(&weaponId);
		bstream->read(&rtt);
		bstream->read(&weaponType);
		bstream->read(&unk_16);
		bstream->read(&unk_17);
		bstream->read(&unk_18);
	};

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
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

		if (weaponUsed.weaponType != BOMB && srcPlayer->isDead())
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

			// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
			switch (hitLocation)
			{
			case HEAD:
				damage *= 1;
				break;
			case BODY:
				damage *= 0.9;
				break;
			case L_LEG:
			case R_LEG:
			case L_ARM:
			case R_ARM:
				damage *= 0.8;
				break;
			case L_FEET:
			case R_FEET:
			case L_HAND:
			case R_HAND:
				damage *= 0.6;
				break;
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

			dstPlayer->takeHealth(damage);

			if (const auto applyEffect = (rand() % 100) <= weaponUsed.effectChance; applyEffect && !isSameTeam)
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
			hitType, hitLocation, dstPlayer->getHealth(), damage, weaponId, rtt, weaponType, unk_16, srcPlayer->getStreak() + 1, unk_18, effectId);

		if (areSkillsEnabled)
		{
			const auto isValidWeapon = !isTrapWeapon 
				&& ((weaponUsed.weaponType == WeaponType::MELEE) || (weaponUsed.weaponType == WeaponType::RIFLE) || (weaponUsed.weaponType == WeaponType::LAUNCHER));

			if (isValidWeapon)
			{
				const auto skillPointsToAdd = (hitLocation == HEAD) ? 10 : 5;

				dstPlayer->getSkillManager()->addSkillPoints(skillPointsToAdd);
			}

			if (dstPlayer->getSkillManager()->hasActiveSkillCard())
			{
				const auto shouldDisableOnBeingAttacked = dstPlayer->getSkillManager()->getActiveSkillCard()->shouldDisableOnBeingAttacked();

				if (shouldDisableOnBeingAttacked)
				{
					dstPlayer->getSkillManager()->deactivateSkillCard();
				}
			}
		}

		if (dstPlayer->isDead())
		{
			if (dstPlayer->getWeaponManager()->getHasEquippedMachineGun())
			{
				dstPlayer->getWeaponManager()->unequipMachineGun();
			}

			srcPlayer->getEntityManager()->addKill(entityId);

			roomSession->getGameMode()->onPlayerKill(srcPlayer, dstPlayer, weaponUsed, hitLocation);
			roomSession->relayPlaying<GCGameState>(dstId, hitLocation == 0 ? 28 : 17, weaponId, srcId);
		}
	}

	void hitEmpty(const uint32_t weaponId, const RoomSessionPlayer::Ptr& srcPlayer) const
	{
		const auto roomSession = srcPlayer->getRoomSession();

		roomSession->relayPlaying<GCHit>(srcPlayerId, 0, unk_03, srcPosX, srcPosY, srcPosZ, dstPosX, dstPosY, dstPosZ, entityId,
			hitType, hitLocation, 0, 0, weaponId, rtt, weaponType, unk_16, srcPlayer->getStreak() + 1, unk_18, 0);
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
	U8 hitLocation; //149

	U32 weaponId; //96
	U64 rtt; //104
	U8 weaponType; //112
	U8 unk_16; //150
	U8 unk_17; //151
	U32 unk_18; //152

	TNL_DECLARE_CLASS(CGHit);
};
#endif