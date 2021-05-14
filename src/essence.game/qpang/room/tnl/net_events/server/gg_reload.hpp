#ifndef GG_RELOAD_HPP
#define GG_RELOAD_HPP

#include "GameNetEvent.h"

class GGReload : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GGReload() : GameNetEvent{ GG_RELOAD, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	U32 unk_01;
	U8 unk_02;

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(unk_01);
		bstream->write(unk_02);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GGReload);
};

#endif // !GC_ARRANGED_ACCEPT_HPP