#ifndef GC_PVE_EVENT_OBJECT_HPP
#define GC_PVE_EVENT_OBJECT_HPP

#include "GameNetEvent.h"

class GCPvEEventObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEEventObject() : GameNetEvent{ GC_PVE_EVENT_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 unk_01;

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(unk_01);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEEventObject);
};

#endif // !GC_ARRANGED_ACCEPT_HPP