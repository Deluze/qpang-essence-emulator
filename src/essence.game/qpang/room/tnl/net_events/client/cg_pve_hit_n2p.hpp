#ifndef CG_PVE_HIT_N2P_H
#define CG_PVE_HIT_N2P_H

#include "GameNetEvent.h"
class CGPvEHitN2P : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEHitN2P() : GameNetEvent{ CG_PVE_HIT_N2P, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

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
	U64 unk_11;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) 
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
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CGPvEHitN2P);
};
#endif