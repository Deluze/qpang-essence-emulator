#ifndef GC_PVE_AREA_TRIGGER_INIT_HPP
#define GC_PVE_AREA_TRIGGER_INIT_HPP

#include "GameNetEvent.h"

class GCPvEAreaTriggerInit : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEAreaTriggerInit() : GameNetEvent{ GC_PVEA_AREA_TRIGGER_INIT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;
	U32 unk_05;
	U8 unk_06;

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(unk_01);
		bstream->write(unk_02);
		bstream->write(unk_03);
		bstream->write(unk_04);
		bstream->write(unk_05);
		bstream->write(unk_06);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEAreaTriggerInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP