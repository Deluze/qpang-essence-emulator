#ifndef CG_PVE_HIT_OBJECT_H
#define CG_PVE_HIT_OBJECT_H

#include "GameNetEvent.h"
class CGPvEHitObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U8 cmd;

	CGPvEHitObject() : GameNetEvent{ CG_PVE_HIT_OBJECT, GuaranteedOrdered, DirClientToServer} {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&cmd);
	}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		// TODO: Handle CGPvEHitObject.
		std::cout << "Reveived an unhandled CGPvEHitObject event." << std::endl;
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitObject>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitObject);
};
#endif