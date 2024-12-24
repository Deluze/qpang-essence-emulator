#ifndef GC_SHOOP_EVENT_HPP
#define GC_SHOOP_EVENT_HPP

#include "GameNetEvent.h"

class GCShoot : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	GCShoot() : GameNetEvent{ GC_SHOOT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	GCShoot(U32 playerId, U32 itemId, F32 srcX, F32 srcY, F32 srcZ, F32 dirX, F32 dirY, F32 dirZ, U32 entityId, U32 isP2P) : 
		GameNetEvent{ GC_SHOOT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient },
		playerId(playerId),
		itemId(itemId),
		unk_03(0xFFFFFFFF),
		srcX(srcX),
		srcY(srcY),
		srcZ(srcZ),
		dirX(dirX),
		dirY(dirY),
		dirZ(dirZ),
		entityId(entityId),
		isP2P(isP2P) {}


	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(playerId);
		bstream->write(itemId);
		bstream->write(unk_03);
		bstream->write(srcX);
		bstream->write(srcY);
		bstream->write(srcZ);
		bstream->write(dirX);
		bstream->write(dirY);
		bstream->write(dirZ);
		bstream->write(entityId);
		bstream->write(isP2P);
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 playerId; // 120
	U32 itemId; // 128
	U32 unk_03; // 116
	F32 srcX; // 92
	F32 srcY; // 96
	F32 srcZ; // 100
	F32 dirX; // 104
	F32 dirY; // 108
	F32 dirZ; // 112
	U32 entityId; // 124
	U32 isP2P; // 88

	TNL_DECLARE_CLASS(GCShoot);
};


#endif // !GC_ARRANGED_ACCEPT_HPP