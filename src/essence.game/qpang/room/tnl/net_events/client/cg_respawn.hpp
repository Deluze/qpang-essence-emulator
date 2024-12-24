#ifndef CG_RESPAWN_H
#define CG_RESPAWN_H

#include "GameNetEvent.h"

#include "qpang/Game.h"

class GCRespawn;

class CGRespawn : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	enum CMD : U32 
	{
		OUR = 2,
		OTHER = 4,
	};

	CGRespawn() : GameNetEvent{ CG_RESPAWN, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (auto session = roomPlayer->getRoomSessionPlayer(); session != nullptr)
			{
				// Can respawn is used for PVE. If the player doesn't have any gold coins left
				// the player shouldn't be able to respawn.
				if (session->isDead() || !session->canRespawn())
					return;

				auto& spawn = Game::instance()->getSpawnManager()->getRandomTeleportSpawn(roomPlayer->getRoom()->getMap());

				auto roomSession = session->getRoomSession();
				roomSession->relayPlaying<GCRespawn>(player->getId(), session->getCharacter(), 2, spawn.x, spawn.y, spawn.z);
			}
		}
	}

	void process(EventConnection* ps) 
	{
		post<CGRespawn>(ps);
	};

	U32 unk_01;
	U8 unk_02;
	U32 unk_03;

	TNL_DECLARE_CLASS(CGRespawn);
};
#endif