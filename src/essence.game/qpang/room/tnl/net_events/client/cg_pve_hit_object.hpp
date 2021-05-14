#ifndef CG_PVE_HIT_OBJECT_H
#define CG_PVE_HIT_OBJECT_H

#include "GameNetEvent.h"
class CGPvEHitObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEHitObject() : GameNetEvent{ CG_PVE_HIT_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U8 cmd;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&cmd);
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CGPvEHitObject);
};
#endif