#ifndef CC_CHARM_H
#define CC_CHARM_H

#include "GameNetEvent.h"

class CCCharm final : public GameNetEvent
{
public:
	U32 playerOneId = 0;	// 92
	U32 unk_02 = 0;			// 96
	U32 unk_03 = 0;			// 100, must be 3?
	U32 playerTwoId = 0;	// 104
	U8 unk_05 = 0;			// 88

	CCCharm() : GameNetEvent{ CC_CHARM, GuaranteedOrdered, DirAny } {}
	CCCharm(const uint32_t playerOneId, const uint32_t playerTwoId, const uint32_t unk_03)
		: GameNetEvent{ CC_CHARM, GuaranteedOrdered, DirAny },
		playerOneId(playerOneId),
		unk_03(unk_03),
		playerTwoId(playerTwoId)
	{}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(playerOneId);
		bstream->write(unk_02);
		bstream->write(unk_03);
		bstream->write(playerTwoId);
		bstream->write(unk_05);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&playerOneId);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&playerTwoId);
		bstream->read(&unk_05);
	}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		printf("[INCOMING CC_CHARM] PlayerOneId: %u, Unk02: %u, Unk03: %u, PlayerTwoId:%u, Unk05: %u\n",
			playerOneId, unk_02, unk_03, playerTwoId, unk_05);
	}

	void process(EventConnection* ps) override
	{
		post<CCCharm>(ps);
	}

	TNL_DECLARE_CLASS(CCCharm);
};

#endif