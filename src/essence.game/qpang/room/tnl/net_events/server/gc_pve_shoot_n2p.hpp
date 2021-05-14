#ifndef GC_PVE_SHOOT_N2P_HPP
#define GC_PVE_SHOOT_N2P_HPP

#include "GameNetEvent.h"

class GCPvEShootN2P : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEShootN2P() : GameNetEvent{ GC_PVE_SHOOT_N2P, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEShootN2P);
};

#endif // !GC_ARRANGED_ACCEPT_HPP