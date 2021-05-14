#ifndef GC_MOTION_HPP
#define GC_MOTION_HPP

#include "GameNetEvent.h"
#include "cg_motion.hpp"

class GCMotion : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCMotion() : GameNetEvent{ GC_MOTION, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCMotion(U32 cmd, U32 unk_02, U32 unk_03, U32 unk_04, U32 unk_05, U32 unk_06, U32 unk_07, U32 unk_08, U32 unk_09, U32 playerId, bool isP2P = false) 
		: GameNetEvent{ GC_MOTION, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient },
		cmd(cmd),
		unk_02(unk_02),
		unk_03(unk_03),
		unk_04(unk_04),
		unk_05(unk_05),
		unk_06(unk_06),
		unk_07(unk_07),
		unk_08(unk_08),
		unk_09(unk_09),
		playerId(playerId),
		isP2P(isP2P)
	{
	};

	void pack(EventConnection* conn, BitStream* bstream)
	{
		 bstream->write(cmd);
		 bstream->write(unk_02);
		 bstream->write(unk_03);
		 bstream->write(unk_04);
		 bstream->write(unk_05);
		 bstream->write(unk_06);
		 bstream->write(unk_07);
		 bstream->write(unk_08);
		 bstream->write(unk_09);
		 bstream->write(playerId);
		 bstream->write(isP2P);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 cmd;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;
	U32 unk_05;
	U32 unk_06;
	U32 unk_07;
	U32 unk_08;
	U32 unk_09;
	U32 playerId;
	U32 isP2P;

	TNL_DECLARE_CLASS(GCMotion);
};

#endif // !GC_ARRANGED_ACCEPT_HPP