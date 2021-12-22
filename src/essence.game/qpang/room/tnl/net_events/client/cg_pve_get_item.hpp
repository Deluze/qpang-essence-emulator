#ifndef CG_PVE_GET_ITEM_H
#define CG_PVE_GET_ITEM_H

#include "GameNetEvent.h"

class CGPvEGetItem final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEGetItem() : GameNetEvent{ CG_PVE_GET_ITEM, GuaranteedOrdered, DirClientToServer} {}

	U32 playerId; // 88
	U32 itemUid; // 92

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&playerId);
		bstream->read(&itemUid);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			return;
		}

		if (roomPlayer->isSpectating())
		{
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		if (roomSessionPlayer->isDead())
		{
			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		roomSession->getPveItemManager()->onItemPickup(playerId, itemUid);
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEGetItem>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEGetItem);
};
#endif