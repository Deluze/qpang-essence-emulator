#ifndef GC_PVE_ITEM_INIT_HPP
#define GC_PVE_ITEM_INIT_HPP

#include "GameNetEvent.h"

class GCPvEItemInit : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEItemInit() : GameNetEvent{ GC_PVE_ITEM_INIT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEItemInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP