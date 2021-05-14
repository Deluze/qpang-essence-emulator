#ifndef CG_PVE_GET_ITEM_H
#define CG_PVE_GET_ITEM_H

#include "GameNetEvent.h"
class CGPvEGetItem : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvEGetItem() : GameNetEvent{ CG_PVE_GET_ITEM, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 unk_01;
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

	TNL_DECLARE_CLASS(CGPvEGetItem);
};
#endif