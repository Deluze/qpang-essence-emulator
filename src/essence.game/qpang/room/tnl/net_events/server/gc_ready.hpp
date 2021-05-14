#ifndef GC_READY_HPP
#define GC_READY_HPP

#include "GameNetEvent.h"

class GCReady : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	GCReady() : GameNetEvent{ GC_READY, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCReady(U32 playerId, U32 cmd) : GameNetEvent{ GC_READY, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->playerId = playerId;
		this->cmd = cmd;
	};
	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(playerId);
		bstream->write(cmd);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U64 playerId;
	U8 cmd;

	TNL_DECLARE_CLASS(GCReady);
};


#endif // !GC_ARRANGED_ACCEPT_HPP