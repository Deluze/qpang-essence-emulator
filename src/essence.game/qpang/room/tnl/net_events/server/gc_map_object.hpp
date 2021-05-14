#ifndef GC_MAP_OBJECT_HPP
#define GC_MAP_OBJECT_HPP

#include "GameNetEvent.h"


class GCMapObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCMapObject() : GameNetEvent{ GC_MAPOBJECT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;
	U32 unk_05;
	U32 unk_06;
	U32 unk_07;
	U32 unk_08;

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(unk_01);
		bstream->write(unk_02);
		bstream->write(unk_03);
		bstream->write(unk_04);
		bstream->write(unk_05);
		bstream->write(unk_06);
		bstream->write(unk_07);
		bstream->write(unk_08);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};
	TNL_DECLARE_CLASS(GCMapObject);
};

#endif // !GC_ARRANGED_ACCEPT_HPP