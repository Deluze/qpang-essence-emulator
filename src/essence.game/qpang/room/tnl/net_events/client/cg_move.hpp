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
				if (session->isDead()) 
				{
					return;
				}

				session->setPosition({ xPosition, yPosition, zPosition });
			}

			const auto roomSession = roomPlayer->getRoom()->getRoomSession();

			if (roomSession != nullptr)
			{
				const auto room = roomPlayer->getRoom();
				if (room && room->getMode() == GameMode::PVE)
				{
					// Note: If this checking causes too much lag or whatever, move it back to PveAreaManager::tick method.
					for (const auto& area : roomSession->getPveAreaManager()->getAreas())
					{
						AABBHelper::isPositionInArea(session->getPosition(), area->getMinBound(), area->getMaxBound())
							&& area->getFloorNumber() == session->getFloorNumber()
							? area->onAreaEnter(session)
							: area->onAreaExit(session);
					}
				}

				roomSession->relayPlayingExcept<GCMove>(player->getId(), playerId, cmd, xPosition, yPosition, zPosition, unk_04, unk_05, unk_06, unk_07, unk_08, unk_09, unk_10);
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