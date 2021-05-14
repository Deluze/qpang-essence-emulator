#ifndef GC_ESSENCE_HPP
#define GC_ESSENCE_HPP

#include "GameNetEvent.h"

#include "cg_essence.hpp"

class GCEssence : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCEssence() : GameNetEvent{ CG_ESSENCE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCEssence(U32 unk_01, U32 unk_02, U32 unk_03, U32 unk_04, U32 unk_05, U32 unk_06, U32 unk_07, U32 unk_08, U32 unk_09) : GameNetEvent{ CG_ESSENCE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->unk_01 = unk_01;
		this->unk_02 = unk_02;
		this->unk_03 = unk_03;
		this->unk_04 = unk_04;
		this->unk_05 = unk_05;
		this->unk_06 = unk_06;
		this->unk_07 = unk_07;
		this->unk_08 = unk_08;
		this->unk_09 = unk_09;
		this->isP2P = false;
	};

	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;
	U32 unk_05;
	U32 unk_06;
	U32 unk_07;
	U32 unk_08;
	U32 unk_09;
	U32 isP2P;

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
		bstream->write(isP2P);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCEssence);
};

#endif // !GC_ARRANGED_ACCEPT_HPP