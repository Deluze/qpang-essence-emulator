#ifndef GC_CHARM_HPP
#define GC_CHARM_HPP

#include "GameNetEvent.h"

class GCCharm : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCCharm() : GameNetEvent{ GC_CHARM, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	U32 unk_01;
	U32 unk_02;

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(unk_01);
		bstream->write(unk_02);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCCharm);
};

#endif // !GC_ARRANGED_ACCEPT_HPP