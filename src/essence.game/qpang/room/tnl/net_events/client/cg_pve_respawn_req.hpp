#ifndef CG_PVE_RESPAWN_REQ_H
#define CG_PVE_RESPAWN_REQ_H

#include "GameNetEvent.h"

#include "qpang/Game.h"

class CGPvERespawnReq final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	bool canRespawn; // 44

	CGPvERespawnReq() : GameNetEvent{ CG_PVE_RESPAWN_REQ, GuaranteedOrdered, DirClientToServer} {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (auto session = roomPlayer->getRoomSessionPlayer(); session != nullptr)
			{
				if (canRespawn)
				{
					// The client told us it can respawn...
					// Yeah, and I am the kerstman, let's check that, before making it the cat wise

					if (player->getCoins() >= 100)
					{
						session->setCanRespawn(true);
						player->removeCoins(100);
					}
				}
				else
				{
					session->setPermanentlyDead(true);

					if (auto roomSession = session->getRoomSession(); roomSession != nullptr)
					{
						if (roomSession->areAllPlayersPermanentlyDead())
							roomSession->finishPveGame(false);
					}
				}
			}
		}
	}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		canRespawn = bstream->readFlag();
	}

	void process(EventConnection* ps) override
	{
		post<CGPvERespawnReq>(ps);
	}

	TNL_DECLARE_CLASS(CGPvERespawnReq);
};
#endif