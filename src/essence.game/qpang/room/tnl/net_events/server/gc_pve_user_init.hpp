#ifndef GC_PVE_USER_INIT_HPP
#define GC_PVE_USER_INIT_HPP

#include "GameNetEvent.h"

class GCPvEUserInit : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEUserInit() : GameNetEvent{ GC_PVE_USER_INIT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEUserInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP