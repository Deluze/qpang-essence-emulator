#ifndef GC_PVE_START_HPP
#define GC_PVE_START_HPP

#include "GameNetEvent.h"

class GCPvEStart : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEStart() : GameNetEvent{ GC_PVE_START, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};
	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEStart);
};

#endif // !GC_ARRANGED_ACCEPT_HPP