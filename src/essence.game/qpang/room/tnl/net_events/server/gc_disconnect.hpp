#ifndef GC_DISCONNECT_HPP
#define GC_DISCONNECT_HPP

#include "GameNetEvent.h"

class GCDisconnect : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCDisconnect() : GameNetEvent{ CG_DISCONNECT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCDisconnect);
};

#endif // !GC_ARRANGED_ACCEPT_HPP