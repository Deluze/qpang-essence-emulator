#ifndef GC_PVE_AREA_TRIGGER_INIT_HPP
#define GC_PVE_AREA_TRIGGER_INIT_HPP

#include "GameNetEvent.h"

class GCPvEAreaTriggerInit final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 areaId = 0; // 88

	F32 minBoundX = 0.00f; // 92
	F32 minBoundZ = 0.00f; // 96

	F32 maxBoundX = 0.00f; // 100
	F32 maxBoundZ = 0.00f; // 104

	U8 cmd = 1;  // 108 Create area = 1

	GCPvEAreaTriggerInit() : GameNetEvent{ GC_PVEA_AREA_TRIGGER_INIT, GuaranteedOrdered, DirServerToClient } {}

	explicit GCPvEAreaTriggerInit(const std::shared_ptr<PveArea>& pveArea)
		: GameNetEvent{ GC_PVEA_AREA_TRIGGER_INIT, GuaranteedOrdered, DirServerToClient },
		areaId(pveArea->getId()),
		minBoundX(pveArea->getMinBound().x),
		minBoundZ(pveArea->getMinBound().z),
		maxBoundX(pveArea->getMaxBound().x),
		maxBoundZ(pveArea->getMaxBound().z)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(areaId);
		bstream->write(minBoundX);
		bstream->write(minBoundZ);
		bstream->write(maxBoundX);
		bstream->write(maxBoundZ);
		bstream->write(cmd);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEAreaTriggerInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP