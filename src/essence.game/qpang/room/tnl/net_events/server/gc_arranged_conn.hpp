#ifndef GC_ARRANGED_CONN_HPP
#define GC_ARRANGED_CONN_HPP

#include "GameNetEvent.h"
#include <string>

class GCArrangedConn : public GameNetEvent
{
	typedef NetEvent Parent;
	typedef GameConnection Connection;
public:

	GCArrangedConn() : GameNetEvent{ GC_ARRANGED_CONN, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};
	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCArrangedConn);
};

#endif // !GC_ARRANGED_ACCEPT_HPP