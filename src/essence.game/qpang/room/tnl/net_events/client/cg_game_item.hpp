#ifndef CG_GAME_ITEM_H
#define CG_GAME_ITEM_H

#include "GameNetEvent.h"

class CGGameItem : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	enum CMD : U8
	{
		PICKUP = 1,
		SPAWN = 6,
		REFILL_AMMO = 14
	};

	CGGameItem() : GameNetEvent{ CG_GAME_ITEM, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&unk_02); // cmd?
		bstream->read(&unk_03);	// always 1
		bstream->read(&unk_04);	// always 0
		bstream->read(&unk_05);	// always 1
		bstream->read(&unk_06); // always 0
		bstream->read(&uid); // identifier of this game item in the map
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer(); roomSessionPlayer != nullptr)
			{
				if (roomSessionPlayer->isDead()) // game still fires event if game item spawns on top of a dead body.
					return;

				roomSessionPlayer->getRoomSession()->getItemManager()->onPickUp(roomSessionPlayer, uid);
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGGameItem>(ps);
	};

	U32 playerId;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;
	U32 unk_05;
	U32 unk_06;
	U32 uid; // uid of the game item the player picks up

	TNL_DECLARE_CLASS(CGGameItem);
};
#endif