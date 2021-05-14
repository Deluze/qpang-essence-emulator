#ifndef GC_PVE_DOOR_HPP
#define GC_PVE_DOOR_HPP

#include "GameNetEvent.h"


class GCPvEDoor : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEDoor() : GameNetEvent{ GC_PVE_DOOR, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 unk_01;

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(unk_01);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEDoor);
};

#endif // !GC_ARRANGED_ACCEPT_HPP