#ifndef GC_TARGET_HPP
#define GC_TARGET_HPP

#include "GameNetEvent.h"

class GCTarget : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCTarget() : GameNetEvent{ GC_TARGET, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient } {};

	GCTarget(U32 cmd, U32 playerId, U32 targetId) : GameNetEvent{ GC_TARGET, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient } 
	{
		this->cmd = cmd;
		this->playerId = playerId;
		this->targetId = targetId;
	};

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(cmd);
		bstream->write(playerId);
		bstream->write(targetId);
		bstream->write(U32(0)); // LOBYTE(result) = BitStream::Read(v3, 32u, &a3);
		// 1 will crash the game ^
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 cmd;
	U32 playerId;
	U32 targetId;

	TNL_DECLARE_CLASS(GCTarget);
};

#endif // !GC_ARRANGED_ACCEPT_HPP