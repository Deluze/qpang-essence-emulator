#ifndef GC_PVE_AREA_TRIGGER_INIT_HPP
#define GC_PVE_AREA_TRIGGER_INIT_HPP

#include "GameNetEvent.h"

class GCPvEAreaTriggerInit final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	struct PveArea
	{
		float beginPositionX;
		float beginPositionZ;
		float endPositionX;
		float endPositionZ;
	};

	U32 areaId = 0; // 88

	F32 beginPositionX = 0.00f; // 92
	F32 beginPositionZ = 0.00f; // 96

	F32 endPositionX = 0.00f; // 100
	F32 endPositionZ = 0.00f; // 104

	U8 cmd = 1;  // 108 Create area = 1

	GCPvEAreaTriggerInit() : GameNetEvent{ GC_PVEA_AREA_TRIGGER_INIT, GuaranteedOrdered, DirServerToClient } {}

	GCPvEAreaTriggerInit(const uint32_t areaId, const PveArea area)
		: GameNetEvent{ GC_PVEA_AREA_TRIGGER_INIT, GuaranteedOrdered, DirServerToClient },
		areaId(areaId),
		beginPositionX(area.beginPositionX),
		beginPositionZ(area.beginPositionZ),
		endPositionX(area.endPositionX),
		endPositionZ(area.endPositionZ)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(areaId);
		bstream->write(beginPositionX);
		bstream->write(beginPositionZ);
		bstream->write(endPositionX);
		bstream->write(endPositionZ);
		bstream->write(cmd);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEAreaTriggerInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP