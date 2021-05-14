#ifndef GC_RESPAWN_HPP
#define GC_RESPAWN_HPP

#include "GameNetEvent.h"

class GCRespawn : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	GCRespawn() : GameNetEvent{ GC_RESPAWN, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCRespawn(U32 playerId, U32 characterId, U32 cmd, F32 x = 0xFF, F32 y = 0xFF, F32 z = 0xFF, U8 isVip = false) : GameNetEvent{ GC_RESPAWN, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->playerId = playerId;
		this->cmd = cmd;
		this->characterIdentifier = characterId,
		this->x = x;
		this->y = y;
		this->z = z;
		this->isVip = isVip;
	};
	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(cmd);
		bstream->write(playerId);
		bstream->write(characterIdentifier);
		bstream->write(x);
		bstream->write(y);
		bstream->write(z);
		bstream->write(isVip);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 playerId;
	U32 cmd;
	U32 characterIdentifier;
	F32 x;
	F32 y;
	F32 z;
	U8 isVip;

	TNL_DECLARE_CLASS(GCRespawn);
};

#endif // !GC_ARRANGED_ACCEPT_HPP