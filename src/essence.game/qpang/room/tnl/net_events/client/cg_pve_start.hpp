#ifndef CG_PVE_START_H
#define CG_PVE_START_H

#include "GameNetEvent.h"
class CGPvEStart final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 playerId; //88
	U8 unk_01; //92 boolean?

	CGPvEStart() : GameNetEvent{ CG_PVE_START, GuaranteedOrdered, DirClientToServer} {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&playerId);
		bstream->read(&unk_01);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		if (const auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (player->getId() != roomPlayer->getRoom()->getMasterId())
			{
				return;
			}

			roomPlayer->getRoom()->start();
		}
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEStart>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEStart);
};
#endif