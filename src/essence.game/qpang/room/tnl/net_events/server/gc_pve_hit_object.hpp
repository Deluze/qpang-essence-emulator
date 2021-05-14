#ifndef GC_PVE_HIT_OBJECT_HPP
#define GC_PVE_HIT_OBJECT_HPP

#include "GameNetEvent.h"

class GCPvEHitObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEHitObject() : GameNetEvent{ GC_PVE_HIT_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};
	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};
	TNL_DECLARE_CLASS(GCPvEHitObject);
};

#endif // !GC_ARRANGED_ACCEPT_HPP