#ifndef CS_RTT_REQUEST_H
#define CS_RTT_REQUEST_H

#include "GameNetEvent.h"

class CSRttRequest : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CSRttRequest() : GameNetEvent{ CS_RTT_REQUEST, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;


	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&unk_04);
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CSRttRequest);
};
#endif