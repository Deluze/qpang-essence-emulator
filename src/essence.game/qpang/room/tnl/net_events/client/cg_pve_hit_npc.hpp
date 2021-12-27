#ifndef CG_PVE_HIT_NPC_H
#define CG_PVE_HIT_NPC_H

#include "GameNetEvent.h"
#include "gc_pve_hit_npc.hpp"

class CGPvEHitNpc final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 playerId; // 140
	U32 targetNpcUid; // 144
	U32 unk_03; // 88
	F32 impactPosX; // 116
	F32 impactPosY; // 120
	F32 impactPosZ; // 124
	F32 impactOffsetPosX; // 128
	F32 impactOffsetPosY; // 132
	F32 impactOffsetPosZ; // 136
	U32 entityId; // 92 possible entity id?
	U8 unk_11; // 148 
	U8 bodyPartId; // 149
	U32 weaponItemId; // 96
	U64 weaponCardId; // 108 104
	U8 weaponType; // 112
	U32 bodyPartIdCopy; // 156
	U8 hitLocation; // 150
	U8 unk_18; // 151
	U32 unk_19; // 152

	CGPvEHitNpc() : GameNetEvent{ CG_PVE_HIT_NPC, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}
	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&playerId);
		bstream->read(&targetNpcUid);
		bstream->read(&unk_03);
		bstream->read(&impactPosX);
		bstream->read(&impactPosY);
		bstream->read(&impactPosZ);
		bstream->read(&impactOffsetPosX);
		bstream->read(&impactOffsetPosY);
		bstream->read(&impactOffsetPosZ);
		bstream->read(&entityId);
		bstream->read(&unk_11);
		bstream->read(&bodyPartId);
		bstream->read(&weaponItemId);
		bstream->read(&weaponCardId);
		bstream->read(&weaponType);
		bstream->read(&bodyPartIdCopy);
		bstream->read(&hitLocation);
		bstream->read(&unk_18);
		bstream->read(&unk_19);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		/*std::cout << std::endl;
		std::cout << "============ CGPvEHitNpc Report ============" << std::endl;
		std::cout << "playerId: " << (int)playerId << std::endl;
		std::cout << "targetNpcUid: " << (int)targetNpcUid << std::endl;
		std::cout << std::endl;
		std::cout << "unk_03: " << (int)unk_03 << std::endl;
		std::cout << std::endl;
		std::cout << "impactPosX: " << (float)impactPosX << std::endl;
		std::cout << "impactPosY: " << (float)impactPosY << std::endl;
		std::cout << "impactPosZ: " << (float)impactPosZ << std::endl;
		std::cout << std::endl;
		std::cout << "impactOffsetPosX: " << (float)impactOffsetPosX << std::endl;
		std::cout << "impactOffsetPosY: " << (float)impactOffsetPosY << std::endl;
		std::cout << "impactOffsetPosZ: " << (float)impactOffsetPosZ << std::endl;
		std::cout << std::endl;
		std::cout << "entityId: " << (int)entityId << std::endl;
		std::cout << "unk_11: " << (int)unk_11 << std::endl;
		std::cout << std::endl;
		std::cout << "bodyPartId: " << (int)bodyPartId << std::endl;
		std::cout << std::endl;

		std::cout << "weaponItemId: " << (int)weaponItemId << std::endl;
		std::cout << "weaponCardId: " << (int)weaponCardId << std::endl;
		std::cout << "weaponType: " << (int)weaponType << std::endl;

		std::cout << std::endl;

		std::cout << "bodyPartIdCopy: " << (int)bodyPartIdCopy << std::endl;
		std::cout << std::endl;
		std::cout << "hitLocation: " << (int)hitLocation << std::endl;
		std::cout << std::endl;
		std::cout << "unk_18: " << (int)unk_18 << std::endl;
		std::cout << "unk_19: " << (int)unk_19 << std::endl;
		std::cout << "============================================" << std::endl;*/

		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		// Check if player is dead, you can not deal damage when you're dead.
		if (roomSessionPlayer->isDead())
		{
			return;
		}

		const auto targetNpc = roomSession->getNpcManager()->findNpcByUid(targetNpcUid);

		// Check if npc exists.
		if (targetNpc == nullptr)
		{
			return;
		}

		// Check if the hit npc is dead, dead npc's can not take MORE damage.
		if (targetNpc->isDead())
		{
			return;
		}

		// Make sure the player has the weapon that is used.
		if (const auto hasWeapon = roomSessionPlayer->getWeaponManager()->hasWeapon(weaponItemId); !hasWeapon)
		{
			return;
		}

		// Make sure the player owns the actual weapon card.
		if (const auto hasWeaponCard = player->getInventoryManager()->hasCard(weaponCardId); !hasWeaponCard)
		{
			return;
		}

		// TODO: Check if weaponType matches with the used weapon.

		if (!targetNpc->hasBodyPart(bodyPartId))
		{
			return;
		}

		// TODO: At this point we know the player & npc exists and both are not dead,
		// TODO: more checks probably need to happen but for now we can relay the damage.

		const auto impactPos = Position{ impactPosX, impactPosY, impactPosZ };
		const auto impactOffsetPos = Position{ impactOffsetPosX, impactOffsetPosY, impactOffsetPosZ };

		const auto weaponUsed = Game::instance()->getWeaponManager()->get(weaponItemId);

		const auto cgPvEHitNpcData = CGPvEHitNpcData
		{
			roomSessionPlayer,
			targetNpc,
			unk_03,
			impactPos,
			impactOffsetPos,
			entityId,
			unk_11,
			bodyPartId,
			weaponUsed,
			weaponCardId,
			static_cast<eWeaponType>(weaponType),
			static_cast<eHitLocation>(hitLocation),
			unk_18,
			unk_19
		};

		roomSession->getNpcManager()->onCGPvEHitNpc(cgPvEHitNpcData);
		targetNpc->setLastAttackerId(player->getId());
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitNpc>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitNpc);
};
#endif