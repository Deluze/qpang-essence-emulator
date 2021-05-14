#ifndef CG_PVE_EVENT_OBJECT_H
#define CG_PVE_EVENT_OBJECT_H

#include "GameNetEvent.h"
class CGPvEEventObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEEventObject() : GameNetEvent { CG_PVE_EVENT_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 eventCmd;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->read(&eventCmd);
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CGPvEEventObject);
};
#endif