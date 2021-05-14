#ifndef GC_PVE_OBJECT_INIT_HPP
#define GC_PVE_OBJECT_INIT_HPP

#include "GameNetEvent.h"


class GCPvEObjectInit : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEObjectInit() : GameNetEvent{ GC_PVE_OBJECT_INIT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};
	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};
	TNL_DECLARE_CLASS(GCPvEObjectInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP