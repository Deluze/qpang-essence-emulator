#ifndef CC_CHARM_H
#define CC_CHARM_H

#include "GameNetEvent.h"

class CCCharm : public GameNetEvent
{
public:
	CCCharm() : GameNetEvent{ CC_CHARM, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	TNL::U32 unk_01;
	TNL::U32 unk_02;
	TNL::U32 unk_03;
	TNL::U32 unk_04;
	TNL::U8 unk_05;

	void pack(EventConnection* conn, BitStream* bstream)
	{

	};

	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&unk_04);
		bstream->read(&unk_05);

		std::cout << "[CC_CHARM] Unpacking..\n";

		std::cout << "[CC_CHARM] unk_01: " << unk_01 << ", unk_02: " << unk_02 << ", unk_03: " << unk_03 << ", unk_04: " << unk_04 << ", unk_05: " << unk_05 << "\n";
	};

	void process(EventConnection* ps)
	{
	};

	TNL_DECLARE_CLASS(CCCharm);
};

#endif