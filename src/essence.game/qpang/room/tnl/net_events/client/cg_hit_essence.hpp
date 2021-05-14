#ifndef CG_HIT_ESSENCE_H
#define CG_HIT_ESSENCE_H

#include "GameNetEvent.h"

#include "qpang/room/tnl/net_events/server/gc_hit_essence.hpp"

class CGHitEssence : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGHitEssence() : GameNetEvent{ CG_HIT_ESSENCE, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	enum CMD : uint32_t
	{
		ESSENCE_THROW = 1,
		ESSENCE_PICK_UP = 2,
		ESSENCE_DROP = 3,
	};

	U32 playerId;
	U32 playerHolderId;
	U32 unk_03;
	F32 x;
	F32 y;
	F32 z;
	U8 unk_07;
	U32 cmd;
	U8 unk_09;
	U32 unk_10;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->read(&playerId);
		bstream->read(&playerHolderId);
		bstream->read(&unk_03);
		bstream->read(&x);
		bstream->read(&y);
		bstream->read(&z);
		bstream->read(&unk_07);
		bstream->read(&cmd);
		bstream->read(&unk_09);
		bstream->read(&unk_10);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (roomPlayer->isSpectating())
				return;

			if (auto session = roomPlayer->getRoomSessionPlayer(); session != nullptr)
			{
				auto roomSession = session->getRoomSession();
				auto essenceHolder = roomSession->getEssenceHolder();

				if (essenceHolder == nullptr)
				{
					if (cmd == CMD::ESSENCE_PICK_UP)
					{
						if (session->isDead())
							return;

						roomSession->setEssenceHolder(session);

						const auto players = roomSession->getPlayingPlayers();

						for (auto& session : players)
							session->post(new GCHitEssence(session->getPlayer()->getId(), player->getId(), CMD::ESSENCE_PICK_UP, x, y, z, unk_03, unk_07));
					}
					else if (cmd == CMD::ESSENCE_DROP)
					{
						roomSession->setEssencePosition({ x, y, z });

						const auto players = roomSession->getPlayingPlayers();

						for (auto& session : players)
							session->post(new GCHitEssence(session->getPlayer()->getId(), player->getId(), CMD::ESSENCE_DROP, x, y, z, unk_03, unk_07));
					}
				}
				else
				{
					if (cmd == CMD::ESSENCE_THROW && player == essenceHolder->getPlayer())
					{
						roomSession->setEssenceHolder(nullptr);

						const auto players = roomSession->getPlayingPlayers();

						for (auto& session : players)
							session->send<GCHitEssence>(session->getPlayer()->getId(), player->getId(), CMD::ESSENCE_THROW, x, y, z, unk_03, unk_07);
					}
					else if (cmd == CMD::ESSENCE_PICK_UP)
					{
						//roomSession->relayPlaying<GCHitEssence>(playerId, playerHolderId, CMD::ESSENCE_PICK_UP, x, y, z, unk_03, unk_07);
					}
				}
			}
		}
	}

	void process(EventConnection* ps) 
	{
		post<CGHitEssence>(ps);
	};

	TNL_DECLARE_CLASS(CGHitEssence);
};

#endif