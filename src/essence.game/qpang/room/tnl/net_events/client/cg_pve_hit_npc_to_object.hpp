#ifndef CG_PVE_HIT_NPC_TO_OBJECT_H
#define CG_PVE_HIT_NPC_TO_OBJECT_H

#include "GameNetEvent.h"

class CGPvEHitNpcToObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	// unk_01 is 0 all of the time so far.
	U64 unk_01; // 88 92
	U32 objectUid; // 96

	CGPvEHitNpcToObject() : GameNetEvent{ CG_PVE_HIT_NPC_TO_OBJECT, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&unk_01);
		bstream->read(&objectUid);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		// TODO: Handle CGPvEHitNpcToObject by sending GCPvEHitNpcToObject.
		//player->getRoomPlayer()->getRoomSessionPlayer()->getRoomSession()->relayPlaying<GCPvEHitNpcToObject>(objectUid, 0, 100);
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitNpcToObject>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitNpcToObject);
};
#endif