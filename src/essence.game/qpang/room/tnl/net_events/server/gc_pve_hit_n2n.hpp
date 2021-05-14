#ifndef GC_PVE_HIT_N2N_HPP
#define GC_PVE_HIT_N2N_HPP

#include "GameNetEvent.h"

class GCPvEHitN2N : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEHitN2N() : GameNetEvent{ GC_PVE_HIT_N2N, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;
	U32 unk_05;
	U32 unk_06;
	U32 unk_07;
	U32 unk_08;
	U8 unk_09;
	U8 unk_10;
	U16 unk_11;

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(unk_01);
		bstream->write(unk_02);
		bstream->write(unk_03);
		bstream->write(unk_04);
		bstream->write(unk_05);
		bstream->write(unk_06);
		bstream->write(unk_07);
		bstream->write(unk_08);
		bstream->write(unk_09);
		bstream->write(unk_10);
		bstream->write(unk_11);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEHitN2N);
};

#endif // !GC_ARRANGED_ACCEPT_HPP