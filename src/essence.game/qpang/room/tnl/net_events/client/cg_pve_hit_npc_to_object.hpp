#ifndef CG_PVE_HIT_NPC_TO_OBJECT_H
#define CG_PVE_HIT_NPC_TO_OBJECT_H

#include "GameNetEvent.h"
class CGPvEHitNpcToObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEHitNpcToObject() : GameNetEvent{ CG_PVE_HIT_NPC_TO_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U64 unk_01;
	U32 unk_02;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CGPvEHitNpcToObject);
};
#endif