#ifndef GC_PVE_OBJECT_MOVE_HPP
#define GC_PVE_OBJECT_MOVE_HPP

#include "GameNetEvent.h"


class GCPvEObjectMove : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid;
	F32 positionX;
	F32 positionY;
	F32 positionZ;
	U32 unk_01;

	GCPvEObjectMove() : GameNetEvent{ GC_PVE_OBJECT_MOVE, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient } {};

	GCPvEObjectMove(U32 objectUid, F32 positionX, F32 positionY, F32 positionZ, U32 speed) : GameNetEvent{ GC_PVE_OBJECT_MOVE, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient }
	{
		this->objectUid = objectUid;
		this->positionX = positionX;
		this->positionY = positionY;
		this->positionZ = positionZ;
		this->unk_01 = speed;
	};

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(objectUid);
		bstream->write(positionX);
		bstream->write(positionY);
		bstream->write(positionZ);
		bstream->write(unk_01);
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEObjectMove);
};

#endif // !GC_ARRANGED_ACCEPT_HPP