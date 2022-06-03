#ifndef GC_PVE_START_HPP
#define GC_PVE_START_HPP

#include "GameNetEvent.h"

class GCPvEStart : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 playerId; // 96

	S32 unk_01 = 2; // 92 Has to be 2, otherwise game simply returns
	U32 unk_02; // 100

	U16 map; // 104
	U16 mode; // 106

	GCPvEStart() : GameNetEvent{ GC_PVE_START, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCPvEStart(Room::Ptr room, uint32_t playerId) : GameNetEvent{ GC_PVE_START, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->playerId = playerId;
		this->map = room->getMap();
		this->mode = room->getMode();
	};

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(playerId);
		bstream->write(unk_01);
		bstream->write(unk_02);
		bstream->write(map);
		bstream->write(mode);
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEStart);
};

#endif // !GC_ARRANGED_ACCEPT_HPP