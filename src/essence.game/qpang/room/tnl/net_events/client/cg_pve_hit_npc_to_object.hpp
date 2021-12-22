#ifndef CG_PVE_HIT_NPC_TO_OBJECT_H
#define CG_PVE_HIT_NPC_TO_OBJECT_H

#include "GameNetEvent.h"
class CGPvEHitNpcToObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U64 unk_01; // 88 92 # WeaponId?
	U32 unk_02; // 96	 # ObjectUid or NpcUid?

	CGPvEHitNpcToObject() : GameNetEvent{ CG_PVE_HIT_NPC_TO_OBJECT, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
	}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		// TODO: Handle CGPvEHitNpcToObject.
		std::cout << "Reveived an unhandled CGPvEHitNpcToObject event." << std::endl;
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitNpcToObject>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitNpcToObject);
};
#endif