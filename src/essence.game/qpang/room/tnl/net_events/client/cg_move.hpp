#ifndef CG_MOVE_H
#define CG_MOVE_H

#include "AABBHelper.h"
#include "GameNetEvent.h"

class GCMove;

class CGMove : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGMove() : GameNetEvent{ CG_MOVE, NetEvent::GuaranteeType::Unguaranteed, NetEvent::DirClientToServer } {};
	U32 playerId = 0;
	U32 cmd = 0;
	F32 xPosition = 0; // x
	F32 yPosition = 0; // y
	F32 zPosition = 0; // z
	F32 unk_04 = 0;
	F32 unk_05 = 0;
	F32 unk_06 = 0;
	F32 unk_07 = 0;
	F32 unk_08 = 0;
	F32 unk_09 = 0;
	F32 unk_10 = 0;
	U32 unk_11 = 1;

	// Yellow side.
	Position* yellowTpOneMinBounds = new Position{ -25, -8, 8 };
	Position* yellowTpOneMaxBounds = new Position{ -28, -11, 10 };

	// Yellow side.
	Position* yellowTpTwoMinBounds = new Position{ -29, -5, 11.3 };
	Position* yellowTpTwoMaxBounds = new Position{ -31.4, -10, 13 };

	// Yellow side.
	Position* blueTpTwoMinBounds = new Position{ 65.50, -5, 15.00 };
	Position* blueTpTwoMaxBounds = new Position{ 67.00, -10, 18.00 };

	void pack(EventConnection* conn, BitStream* bstream) override {};
	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
		bstream->read(&xPosition);
		bstream->read(&yPosition);
		bstream->read(&zPosition);
		bstream->read(&unk_04);
		bstream->read(&unk_05);
		bstream->read(&unk_06);
		bstream->read(&unk_07);
		bstream->read(&unk_08);
		bstream->read(&unk_09);
		bstream->read(&unk_10);
	};

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		if (const auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (roomPlayer->isSpectating())
			{
				return;
			}

			const auto session = roomPlayer->getRoomSessionPlayer();

			if (session != nullptr)
			{
				if (session->isDead()) {// if they want to try and revive themselves
					return;
				}

				session->setPosition({ xPosition, yPosition, zPosition });
			}

			const auto roomSession = roomPlayer->getRoom()->getRoomSession();

			if (roomSession != nullptr)
			{
				roomSession->relayPlayingExcept<GCMove>(player->getId(), playerId, cmd, xPosition, yPosition, zPosition, unk_04, unk_05, unk_06, unk_07, unk_08, unk_09, unk_10);
			}

			const auto mapId = roomPlayer->getRoom()->getMap();

			// Allow teleport spawns.
			if (mapId != 11)
			{
				return;
			}

			const auto playerPosition = new Position{ xPosition, yPosition, zPosition };

			if (
				AABBHelper::isPositionInBetweenBounds(
					*playerPosition, *yellowTpOneMinBounds, *yellowTpOneMaxBounds) ||
				AABBHelper::isPositionInBetweenBounds(
					*playerPosition, *yellowTpTwoMinBounds, *yellowTpTwoMaxBounds))
			{
				// Random blue spawn.
				auto [x, y, z] = Game::instance()->getSpawnManager()->getRandomSpawn(mapId, 1);

				roomSession->relayPlaying<GCRespawn>(player->getId(), session->getCharacter(), 2, x, y, z);
			}
			else if (AABBHelper::isPositionInBetweenBounds(
				*playerPosition, *blueTpTwoMinBounds, *blueTpTwoMaxBounds))
			{
				// Random yellow spawn.
				auto [x, y, z] = Game::instance()->getSpawnManager()->getRandomSpawn(mapId, 2);

				roomSession->relayPlaying<GCRespawn>(player->getId(), session->getCharacter(), 2, x, y, z);
			}
		}
	}

	void process(EventConnection* ps) override
	{
		post<CGMove>(ps);
	}

	TNL_DECLARE_CLASS(CGMove);
};
#endif