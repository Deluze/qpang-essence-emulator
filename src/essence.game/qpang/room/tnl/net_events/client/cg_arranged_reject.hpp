#ifndef CG_ARRANGED_REJECT_HPP
#define CG_ARRANGED_REJECT_HPP

#include "GameNetEvent.h"

class CGArrangedReject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGArrangedReject() : GameNetEvent{ CG_ARRANGED_REJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } 
	{
		unk_04 = new ByteBuffer();
	};

	U32 sourcePlayerId;
	U32 targetPlayerId;
	U32 unk_03;
	ByteBuffer* unk_04;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&sourcePlayerId);
		bstream->read(&targetPlayerId);
		bstream->read(&unk_03);
		bstream->read(unk_04);
	};

	void process(EventConnection* ps) 
	{
		delete unk_04;
	};

	TNL_DECLARE_CLASS(CGArrangedReject);
};
#endif