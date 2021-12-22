#ifndef GC_PVE_AREA_TRIGGER_INIT_HPP
#define GC_PVE_AREA_TRIGGER_INIT_HPP

#include "GameNetEvent.h"

class GCPvEAreaTriggerInit final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 unk_01; // 88	# ObjectUid maybe?
	F32 unk_02; // 92	# PositionX maybe?
	F32 unk_03; // 96	# PositionY maybe?
	F32 unk_04; // 100  # PositionZ maybe?
	F32 unk_05; // 104  # Unknown float.. something with position or direction perhaps?
	U8 unk_06;  // 108

	GCPvEAreaTriggerInit() : GameNetEvent{ GC_PVEA_AREA_TRIGGER_INIT, GuaranteedOrdered, DirServerToClient } {}

	GCPvEAreaTriggerInit(const uint32_t unk_01, const float unk_02, const float unk_03, const float unk_04, const  float unk_05, const uint8_t unk_06)
		: GameNetEvent{ GC_PVEA_AREA_TRIGGER_INIT, GuaranteedOrdered, DirServerToClient },
		unk_01(unk_01),
		unk_02(unk_02),
		unk_03(unk_03),
		unk_04(unk_04),
		unk_05(unk_05),
		unk_06(unk_06)
	{
	}


	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(unk_01);
		bstream->write(unk_02);
		bstream->write(unk_03);
		bstream->write(unk_04);
		bstream->write(unk_05);
		bstream->write(unk_06);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEAreaTriggerInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP