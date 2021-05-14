#ifndef GC_PVE_DESTROY_PARTS_HPP
#define GC_PVE_DESTROY_PARTS_HPP

#include "GameNetEvent.h"

class GCPvEDestroyParts : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEDestroyParts() : GameNetEvent{ GC_PVE_DESTROY_PARTS, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};;

	U32 unk_01;
	U32 unk_02;

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(unk_01);
		bstream->write(unk_02);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};
	TNL_DECLARE_CLASS(GCPvEDestroyParts);
};

#endif // !GC_ARRANGED_ACCEPT_HPP