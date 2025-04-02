#ifndef CG_SYNC_H
#define CG_SYNC_H

#include <iostream>

#include "GameNetEvent.h"

class GCSync;

class CGSync : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGSync() : GameNetEvent{ CG_SYNC, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	U32 cmd;
	U32 playerId;
	U32 entityId;
	F32 xPos;
	F32 yPos;
	F32 zPos;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&cmd);
		bstream->read(&playerId);
		bstream->read(&entityId);
		bstream->read(&xPos);
		bstream->read(&yPos);
		bstream->read(&zPos);
	};

	// TODO: Start keeping track of entity ids.
	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer(); roomSessionPlayer != nullptr)
			{
				if (const auto roomSession = roomSessionPlayer->getRoomSession(); roomSession != nullptr)
				{
					roomSession->relayPlaying<GCSync>(cmd, playerId, entityId, xPos, yPos, zPos, false);
				}
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGSync>(ps);
	};

	TNL_DECLARE_CLASS(CGSync);
};
#endif