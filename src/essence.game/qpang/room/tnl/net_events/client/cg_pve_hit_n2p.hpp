#ifndef CG_PVE_HIT_N2P_H
#define CG_PVE_HIT_N2P_H

#include "GameNetEvent.h"
class CGPvEHitN2P final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEHitN2P() : GameNetEvent{ CG_PVE_HIT_N2P, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {}

	U32 unk_01; // 88 
	U32 unk_02; // 92
	U32 unk_03; // 96
	U32 unk_04; // 100
	U32 unk_05; // 104
	U32 unk_06; // 108
	U32 unk_07; // 112
	U32 unk_08; // 116
	U8 unk_09;  // 120
	U8 unk_10;  // 121
	U64 unk_11; // 128?

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&unk_04);
		bstream->read(&unk_05);
		bstream->read(&unk_06);
		bstream->read(&unk_07);
		bstream->read(&unk_08);
		bstream->read(&unk_09);
		bstream->read(&unk_10);
		bstream->read(&unk_11);
	}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		// TODO: Handle CGPvEHitN2P.
		std::cout << "Reveived an unhandled CGPvEHitN2P event." << std::endl;
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitN2P>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitN2P);
};
#endif