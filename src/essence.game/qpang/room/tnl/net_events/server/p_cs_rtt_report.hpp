#ifndef P_CS_RTT_REPORT_HPP
#define P_CS_RTT_REPORT_HPP

#include "GameNetEvent.h"

class P_CSRttReport : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	P_CSRttReport() : GameNetEvent{ CS_RTT_REPORT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(P_CSRttReport);
};

#endif // !GC_ARRANGED_ACCEPT_HPP