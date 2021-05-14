#ifndef GC_START_HPP
#define GC_START_HPP

#include "GameNetEvent.h"

#include "qpang/room/Room.h"

class GCStart : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	U32 playerId;

	U32 unk_01;
	U32 unk_02;

	U16 map;
	U16 mode;

	GCStart() : GameNetEvent{ GC_START, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCStart(Room::Ptr room, uint32_t playerId) : GameNetEvent{ GC_START, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->playerId = playerId;
		this->map = room->getMap();
		this->mode = room->getMode();
	};

	void pack(EventConnection* conn, BitStream* bstream)
	{	
		bstream->write(playerId);
		bstream->write(2);
		bstream->write(unk_02);
		bstream->write(map);
		bstream->write(mode);
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCStart);
};

#endif // !GC_ARRANGED_ACCEPT_HPP