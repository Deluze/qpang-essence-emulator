#ifndef GC_SYNC_HPP
#define GC_SYNC_HPP

#include "GameNetEvent.h"

class GCSync : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCSync() : GameNetEvent{ GC_SYNC, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCSync(U32 cmd, U32 playerId, U32 entityId, F32 xPos, F32 yPos, F32 zPos, U8 isP2P = false) 
		: GameNetEvent{ GC_SYNC, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient },
		cmd(cmd),
		playerId(playerId),
		entityId(entityId),
		xPos(xPos),
		yPos(yPos),
		zPos(zPos),
		isP2P(isP2P)
	{
	};

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(cmd);
		bstream->write(playerId);
		bstream->write(entityId);
		bstream->write(xPos);
		bstream->write(yPos);
		bstream->write(zPos);
		bstream->write(isP2P);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 cmd;
	U32 playerId;
	U32 entityId;
	F32 xPos;
	F32 yPos;
	F32 zPos;
	U8 isP2P;

	TNL_DECLARE_CLASS(GCSync);
};

#endif // !GC_ARRANGED_ACCEPT_HPP