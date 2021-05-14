#ifndef CG_PVE_HIT_NPC_H
#define CG_PVE_HIT_NPC_H

#include "GameNetEvent.h"
class CGPvEHitNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEHitNpc() : GameNetEvent{ CG_PVE_HIT_NPC, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;
	U32 unk_05;
	U32 unk_06;
	U32 unk_07;
	U32 unk_08;
	U32 unk_09;
	U32 unk_10;
	U8 unk_11;
	U8 unk_12;
	U32 unk_13;
	U64 unk_14;
	U8 unk_15;
	U32 unk_16;
	U8 unk_17;
	U8 unk_18;
	U32 unk_19;


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
		bstream->read(&unk_12);
		bstream->read(&unk_13);
		bstream->read(&unk_14);
		bstream->read(&unk_15);
		bstream->read(&unk_16);
		bstream->read(&unk_17);
		bstream->read(&unk_18);
		bstream->read(&unk_19);
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CGPvEHitNpc);
};
#endif