#ifndef CG_PVE_AREA_TRIGGER_H
#define CG_PVE_AREA_TRIGGER_H

#include "GameNetEvent.h"
class CGPvEAreaTrigger : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEAreaTrigger() : GameNetEvent{ CG_PVE_AREA_TRIGGER, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 flag;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->read(&flag);
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CGPvEAreaTrigger);
};
#endif