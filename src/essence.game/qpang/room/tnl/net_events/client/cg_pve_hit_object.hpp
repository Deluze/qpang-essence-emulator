#ifndef CG_PVE_HIT_OBJECT_H
#define CG_PVE_HIT_OBJECT_H

#include "GameNetEvent.h"

class CGPvEHitObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 playerId;
	U32 objectUid;
	U32 weaponItemId;
	U8 cmd;

	CGPvEHitObject() : GameNetEvent{ CG_PVE_HIT_OBJECT, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&playerId);
		bstream->read(&objectUid);
		bstream->read(&weaponItemId);
		bstream->read(&cmd);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		// TODO: This works but, we need to look at the object type and stuff to properly handle this event.
		//player->getRoomPlayer()->getRoomSessionPlayer()->getRoomSession()->relayPlaying<GCPvEHitObject>(playerId, objectUid, weaponItemId, cmd, 10);
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitObject>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitObject);
};
#endif