#ifndef GC_HIT_ESSENCE_HPP
#define GC_HIT_ESSENCE_HPP

#include "GameNetEvent.h"

class GCHitEssence : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCHitEssence() : GameNetEvent{ GC_HIT_ESSENCE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCHitEssence(uint32_t playerId, uint32_t playerHolderId, uint32_t cmd, F32 x = 0xFF, F32 y = 0xFF, F32 z = 0xFF, uint32_t unk_03 = 0, uint32_t unk_07 = 0) : GameNetEvent{ GC_HIT_ESSENCE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } 
	{
		this->playerId = playerId;
		this->playerHolderId = playerHolderId;
		this->cmd = cmd;
		this->x = x;
		this->y = y;
		this->z = z;
		this->unk_03 = unk_03;
		this->unk_07 = unk_07;
	};
	
	U32 playerId;
	U32 playerHolderId;
	U32 unk_03;
	F32 x;
	F32 y;
	F32 z;
	U8 unk_07;
	U32 cmd;

	void pack(EventConnection* conn, BitStream* bstream)\
	{
		bstream->write(playerId);
		bstream->write(playerHolderId);
		bstream->write(unk_03);
		bstream->write(x);
		bstream->write(y);
		bstream->write(z);
		bstream->write(unk_07);
		bstream->write(cmd);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCHitEssence);
};

#endif // !GC_ARRANGED_ACCEPT_HPP