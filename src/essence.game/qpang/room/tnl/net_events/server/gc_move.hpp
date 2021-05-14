#ifndef GC_MOVE_HPP
#define GC_MOVE_HPP

#include "GameNetEvent.h"
#include "cg_move.hpp"

class GCMove : public GameNetEvent
{

	typedef NetEvent Parent;
public:
	GCMove() : GameNetEvent{ GC_MOVE, NetEvent::GuaranteeType::Unguaranteed, NetEvent::DirServerToClient } {};

	GCMove(U32 playerId, U32 cmd, F32 unk_01, F32 unk_02, F32 unk_03, F32 unk_04, F32 unk_05, F32 unk_06, F32 unk_07, F32 unk_08, F32 unk_09, F32 unk_10)
		: GameNetEvent{ GC_MOVE, NetEvent::GuaranteeType::Unguaranteed, NetEvent::DirServerToClient },
		playerId(playerId),
		cmd(cmd),
		unk_01(unk_01),
		unk_02(unk_02),
		unk_03(unk_03),
		unk_04(unk_04),
		unk_05(unk_05),
		unk_06(unk_06),
		unk_07(unk_07),
		unk_08(unk_08),
		unk_09(unk_09),
		unk_10(unk_10)
	{
	};

	U32 playerId;
	U32 cmd;
	F32 unk_01;
	F32 unk_02;
	F32 unk_03;
	F32 unk_04;
	F32 unk_05;
	F32 unk_06;
	F32 unk_07;
	F32 unk_08;
	F32 unk_09;
	F32 unk_10;
	U8 isP2P = 0;

	virtual void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(playerId);
		bstream->write(cmd);
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
		bstream->write(isP2P);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCMove);
};

#endif // !GC_ARRANGED_ACCEPT_HPP