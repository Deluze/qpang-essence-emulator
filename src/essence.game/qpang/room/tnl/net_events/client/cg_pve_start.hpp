#ifndef CG_PVE_START_H
#define CG_PVE_START_H

#include "GameNetEvent.h"
class CGPvEStart : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 mUid;
	U8 unk_01;
	CGPvEStart() : GameNetEvent{ CG_PVE_START, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->read(&mUid);
		bstream->read(&unk_01);
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CGPvEStart);
};
#endif