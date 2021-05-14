#ifndef P_CS_RTT_RESPONSE_HPP
#define P_CS_RTT_RESPONSE_HPP

#include "GameNetEvent.h"

class P_CSRttResponse : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	P_CSRttResponse() : GameNetEvent{ CS_RTT_RESPONSE, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};
	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(P_CSRttResponse);
};

#endif // !GC_ARRANGED_ACCEPT_HPP