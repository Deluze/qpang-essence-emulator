#ifndef GC_PVE_ITEM_INIT_HPP
#define GC_PVE_ITEM_INIT_HPP

#include "GameNetEvent.h"

class GCPvEItemInit final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 itemId; // 88
	U32 itemUid; // 92
	F32 xPos; // 96
	F32 yPos; // 100
	F32 zPos; // 104

	GCPvEItemInit() : GameNetEvent{ GC_PVE_ITEM_INIT, GuaranteedOrdered, DirServerToClient } {}
	GCPvEItemInit(const uint32_t itemId, const uint32_t itemUid, const Position position)
		: GameNetEvent{ GC_PVE_ITEM_INIT, GuaranteedOrdered, DirServerToClient },
		itemId(itemId),
		itemUid(itemUid),
		xPos(position.x),
		yPos(position.y),
		zPos(position.z)
	{
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(itemId);
		bstream->write(itemUid);
		bstream->write(xPos);
		bstream->write(yPos);
		bstream->write(zPos);
	}

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCPvEItemInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP