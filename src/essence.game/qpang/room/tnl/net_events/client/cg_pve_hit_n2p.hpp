// ReSharper disable CppClangTidyClangDiagnosticFloatConversion
#ifndef CG_PVE_HIT_N2P_H
#define CG_PVE_HIT_N2P_H

#include "GameNetEvent.h"
#include "gc_pve_hit_n2p.hpp"

class CGPvEHitN2P final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 npcUid; // 88
	U32 npcBodyPartId; // 92
	F32 impactPosX; // 96
	F32 impactPosY; // 100
	F32 impactPosZ; // 104
	F32 impactOffsetPosX; // 108
	F32 impactOffsetPosY; // 112
	F32 impactOffsetPosZ; // 116
	U8 unk_09;  // 120
	U8 bodyPartHitLocation;  // 121
	U64 unk_11; // 128?

	CGPvEHitN2P() : GameNetEvent{ CG_PVE_HIT_N2P, GuaranteedOrdered, DirClientToServer } {}

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
	};

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&npcUid);
		bstream->read(&npcBodyPartId);
		bstream->read(&impactPosX);
		bstream->read(&impactPosY);
		bstream->read(&impactPosZ);
		bstream->read(&impactOffsetPosX);
		bstream->read(&impactOffsetPosY);
		bstream->read(&impactOffsetPosZ);
		bstream->read(&unk_09);
		bstream->read(&bodyPartHitLocation);
		bstream->read(&unk_11);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		/*std::cout << std::endl;
		std::cout << "============ CG_PVE_HIT_N2P Report ============" << std::endl;
		std::cout << "npcUid: " << (int)npcUid << std::endl;
		std::cout << "bodyPartId: " << (int)npcBodyPartId << std::endl;
		std::cout << "impactPosX: " << (float)impactPosX << std::endl;
		std::cout << "impactPosY: " << (float)impactPosY << std::endl;
		std::cout << "impactPosZ: " << (float)impactPosZ << std::endl;
		std::cout << "impactOffsetPosX: " << (float)impactOffsetPosX << std::endl;
		std::cout << "impactOffsetPosY: " << (float)impactOffsetPosY << std::endl;
		std::cout << "impactOffsetPosZ: " << (float)impactOffsetPosZ << std::endl;
		std::cout << "unk_09: " << (int)unk_09 << std::endl;
		std::cout << "bodyPartHitLocation: " << (int)bodyPartHitLocation << std::endl;
		std::cout << "unk_11: " << (int)unk_11 << std::endl;
		std::cout << "==============================================" << std::endl;*/

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

		const auto npc = roomSession->getNpcManager()->findNpcByUid(npcUid);

		if (npc == nullptr)
		{
			return;
		}

		// Check if player is dead, the player can not be dealt damage if they are dead.
		if (roomSessionPlayer->isDead() || roomSessionPlayer->isInvincible())
		{
			return;
		}

		// Make sure the body part id from the client exists on the server.
		if (!npc->hasBodyPart(npcBodyPartId))
		{
			return;
		}

		// TODO: Check if the player hit was valid (compare position between npc and player).

		const auto impactPos = Position{ impactPosX, impactPosY, impactPosZ };
		const auto impactOffsetPos = Position{ impactOffsetPosX, impactOffsetPosY, impactOffsetPosZ };

		const auto weaponItemId = npc->getBodyPartById(npcBodyPartId)->weaponItemId;
		const auto weaponUsed = Game::instance()->getWeaponManager()->get(weaponItemId);

		auto damage = weaponUsed.damage;

		// TODO: Different damage modifiers based on character?
		switch (static_cast<BodyPartHitLocation>(bodyPartHitLocation))
		{
		case HEAD:
			damage *= 2.0;
			break;
		case BODY:
		case RIGHT_ARM:
		case RIGHT_HAND:
		case LEFT_ARM:
		case LEFT_HAND:
		case RIGHT_LEG:
		case RIGHT_FEET:
		case LEFT_LEG:
		case LEFT_FEET:
		case SPLASH_1:
		case SPLASH_2:
			break;
		}

		roomSessionPlayer->takeHealth(damage, true);
		roomSession->relayPlaying<GCPvEHitN2P>(npcUid, npcBodyPartId, impactPos, impactOffsetPos, unk_09, bodyPartHitLocation, player->getId(), roomSessionPlayer->getHealth());

		if (roomSessionPlayer->isDead())
		{
			const auto isHeadShot = (bodyPartHitLocation == HEAD);
			const auto gameState = isHeadShot ? CGGameState::KILLFEED_ADD_HEAD : CGGameState::KILLFEED_ADD;

			// Update the killfeed.
			roomSession->relayPlaying<GCGameState>(player->getId(), gameState, weaponUsed.itemId, npcUid);
			roomSession->getGameMode()->onPlayerDeathByNpc(roomSessionPlayer, npc);
		}
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitN2P>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitN2P);
};
#endif