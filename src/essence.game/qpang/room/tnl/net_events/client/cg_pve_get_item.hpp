#ifndef CG_PVE_GET_ITEM_H
#define CG_PVE_GET_ITEM_H

#include "GameNetEvent.h"
#include "gc_game_item.hpp"

class CGPvEGetItem : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEGetItem() : GameNetEvent{ CG_PVE_GET_ITEM, GuaranteedOrdered, DirClientToServer} {}

	U32 unk_01; // 88
	U32 itemUid; // 92

	void pack(EventConnection* conn, BitStream* bstream) {}

	void unpack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->read(&unk_01);
		bstream->read(&itemUid);
	}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		std::cout << "CGPvEGetItem pickup item unk_01: " << unk_01 << ", ItemUid: " << itemUid << std::endl;

		const auto roomSession = player->getRoomPlayer()->getRoomSessionPlayer()->getRoomSession();

		roomSession->relayPlaying<GCGameItem>(GCGameItem::CMD::PICKUP_GAME_ITEM, player->getId(), 1191182352, itemUid, 0);
	}

	void process(EventConnection* ps) 
	{
		post<CGPvEGetItem>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEGetItem);
};
#endif