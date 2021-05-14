#ifndef CG_ROOM_INFO_H
#define CG_ROOM_INFO_H

#include "GameNetEvent.h"
class CGRoomInfo : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGRoomInfo() : GameNetEvent{ CG_ROOM_INFO, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	U32 masterUid; //88

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&masterUid); //88
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CGRoomInfo);
};

#endif