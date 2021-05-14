#ifndef GC_QUEST_HPP
#define GC_QUEST_HPP

#include "GameNetEvent.h"

class GCQuest : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCQuest() : GameNetEvent{ GC_QUEST, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	U32 unk_01;
	U8 cmd;
	U16 unk_02;
	U8 unk_03;

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(unk_01);
		bstream->write(cmd);
		bstream->write(unk_02);
		bstream->write(unk_03);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCQuest);
};

#endif // !GC_ARRANGED_ACCEPT_HPP