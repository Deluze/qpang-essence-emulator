#ifndef CC_CHARM_H
#define CC_CHARM_H

#include "GameNetEvent.h"

class CCCharm : public GameNetEvent
{
public:
	CCCharm() : GameNetEvent{ CC_CHARM, GuaranteedOrdered, DirAny} {}

	U32 unk_01; // 92
	U32 unk_02; // 96
	U32 unk_03; // 100
	U32 unk_04; // 104
	U8 unk_05;	// 88

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(&unk_01);
		bstream->write(&unk_02);
		bstream->write(&unk_03);
		bstream->write(&unk_04);
		bstream->write(&unk_05);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&unk_04);
		bstream->read(&unk_05);
	}

	void handle(GameConnection* conn, Player::Ptr player) override
	{

	}

	void process(EventConnection* ps) override
	{
		post<CCCharm>(ps);
	}

	TNL_DECLARE_CLASS(CCCharm);
};

#endif