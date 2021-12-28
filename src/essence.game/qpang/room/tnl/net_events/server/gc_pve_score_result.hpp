#ifndef GC_PVE_SCORE_RESULT_HPP
#define GC_PVE_SCORE_RESULT_HPP

#include "GameNetEvent.h"

class GCPvEScoreResult final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 unk_01 = 1; // 88 playerId
	U32 unk_02 = 1; // 92
	U32 unk_03 = 1; // 96
	U32 unk_04 = 1; // 100
	U32 unk_05 = 1; // 104
	U32 unk_06 = 1; // 108
	U32 unk_07 = 1; // 112
	U32 unk_08 = 1; // 116
	U32 unk_09 = 1; // 120
	U32 unk_10 = 1; // 124
	U32 unk_11 = 1; // 128
	U32 unk_12 = 1; // 132
	U32 unk_13 = 1; // 136
	U32 unk_14 = 1; // 140

	U8 didSucceed = 1; // 144

	U16 unk_16 = 1; // 146
	U16 unk_17 = 1; // 148

	GCPvEScoreResult() : GameNetEvent{ GC_PVE_SCORE_RESULT, GuaranteedOrdered, DirServerToClient} {}

	void pack(EventConnection* conn, BitStream* bstream) override
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
		bstream->write(unk_12);
		bstream->write(unk_13);
		bstream->write(unk_14);
		bstream->write(didSucceed);
		bstream->write(unk_16);
		bstream->write(unk_17);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEScoreResult);
};

#endif // !GC_ARRANGED_ACCEPT_HPP