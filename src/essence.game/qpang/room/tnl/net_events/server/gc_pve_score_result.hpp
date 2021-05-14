#ifndef GC_PVE_SCORE_RESULT_HPP
#define GC_PVE_SCORE_RESULT_HPP

#include "GameNetEvent.h"

class GCPvEScoreResult : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEScoreResult() : GameNetEvent{ GC_PVE_SCORE_RESULT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEScoreResult);
};

#endif // !GC_ARRANGED_ACCEPT_HPP