#ifndef CG_SHOOT_H
#define CG_SHOOT_H

#include <iostream>
#include "GameNetEvent.h"

#include "qpang/room/RoomPlayer.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

class GCShoot;

class CGShoot : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGShoot() : GameNetEvent{ CG_SHOOT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer } {};

	U32 playerId;
	U32 unk_02;
	U32 unk_03;
	F32 srcX;
	F32 srcY;
	F32 srcZ;
	F32 dirX;
	F32 dirY;
	F32 dirZ;
	U32 entityId;
	U32 itemId;
	U64 cardId;
	U16 cmd;
	U32 unk_11;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&srcX);
		bstream->read(&srcY);
		bstream->read(&srcZ);
		bstream->read(&dirX);
		bstream->read(&dirY);
		bstream->read(&dirZ);
		bstream->read(&entityId);
		bstream->read(&itemId);
		bstream->read(&cardId);
		bstream->read(&cmd);
		bstream->read(&unk_11);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (roomPlayer->isSpectating())
				return;

			if (auto session = roomPlayer->getRoomSessionPlayer(); session != nullptr)
			{
				auto* weaponManager = session->getWeaponManager();
				if (session->isDead() || !weaponManager->canShoot())
					return;

				if (session->isInvincible())
					session->removeInvincibility();
				
				auto playerId = player->getId();
				weaponManager->shoot(entityId);

				if (itemId != 1095434246) // !octo
					session->getEntityManager()->shoot(entityId);
				
				session->getRoomSession()->relayPlayingExcept<GCShoot>(playerId, playerId, itemId, srcX, srcY, srcZ, dirX, dirY, dirZ, entityId, false);
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGShoot>(ps);
	};

	TNL_DECLARE_CLASS(CGShoot);
};
#endif