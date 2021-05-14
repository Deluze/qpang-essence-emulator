#ifndef GC_ARRANGED_ACCEPT_HPP
#define GC_ARRANGED_ACCEPT_HPP

#include "GameNetEvent.h"

class GCArrangedAccept : public GameNetEvent
{
	typedef NetEvent Parent;
	const std::u16string key = u"123456781234567892345672345672345672345672345678";

public:
	GCArrangedAccept() : GameNetEvent{ GC_ARRANGED_ACCEPT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCArrangedAccept(U32 senderId, U32 targetId) : GameNetEvent{ GC_ARRANGED_ACCEPT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->senderId = senderId;
		this->targetId = targetId;
	};
	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(senderId); // ?
		bstream->write(targetId); // target uid
		bstream->write(1);
		bstream->write(1); // possible addresses count.
		bstream->write(U32(222));
		bstream->write(U16(targetId));

		writeByteBuffer(bstream, key, key.size());
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) 
	{
	};

	U32 senderId;
	U32 targetId;

	TNL_DECLARE_CLASS(GCArrangedAccept);
};


#endif // !GC_ARRANGED_ACCEPT_HPP
