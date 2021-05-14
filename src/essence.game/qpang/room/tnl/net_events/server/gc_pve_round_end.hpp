#ifndef GC_PVE_ROUND_END_HPP
#define GC_PVE_ROUND_END_HPP

#include "GameNetEvent.h"

class GCPvERoundEnd : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvERoundEnd() : GameNetEvent{ GC_PVE_ROUND_END, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvERoundEnd);
};

#endif // !GC_ARRANGED_ACCEPT_HPP