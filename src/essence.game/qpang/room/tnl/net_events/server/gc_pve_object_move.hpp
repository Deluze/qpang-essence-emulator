#ifndef GC_PVE_OBJECT_MOVE_HPP
#define GC_PVE_OBJECT_MOVE_HPP

#include "GameNetEvent.h"


class GCPvEObjectMove : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEObjectMove() : GameNetEvent{ GC_PVE_OBJECT_MOVE, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEObjectMove);
};

#endif // !GC_ARRANGED_ACCEPT_HPP