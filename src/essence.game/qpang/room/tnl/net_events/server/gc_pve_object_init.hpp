#ifndef GC_PVE_OBJECT_INIT_HPP
#define GC_PVE_OBJECT_INIT_HPP

#include "GameNetEvent.h"


class GCPvEObjectInit : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectType; // 88
	U32 objectUid; // 92
	F32 xPosition; // 96
	F32 yPosition; // 100
	F32 zPosition; // 104
	U16 unk_01;  // 108

	GCPvEObjectInit() : GameNetEvent{ GC_PVE_OBJECT_INIT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient } {};
	GCPvEObjectInit(U32 objectType, U32 objectUid, F32 xPosition, F32 yPosition, F32 zPosition, U16 unk_01) : GameNetEvent{ GC_PVE_OBJECT_INIT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient }
	{
		this->objectType = objectType;
		this->objectUid = objectUid;
		this->xPosition = xPosition;
		this->yPosition = yPosition;
		this->zPosition = zPosition;
		this->unk_01 = unk_01;
	};

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(objectType);
		bstream->write(objectUid);
		bstream->write(xPosition);
		bstream->write(yPosition);
		bstream->write(zPosition);
		bstream->write(unk_01);
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};
	TNL_DECLARE_CLASS(GCPvEObjectInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP