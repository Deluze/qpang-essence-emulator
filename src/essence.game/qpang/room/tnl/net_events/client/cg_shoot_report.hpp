#ifndef CG_SHOOT_REPORT_H
#define CG_SHOOT_REPORT_H

#include "GameNetEvent.h"
class CGShootReport : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGShootReport() : GameNetEvent{ CG_SHOOTREPORT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;
	U32 unk_05;
	U64 unk_06;
	U16 unk_07;
	U32 unk_08;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&unk_04);
		bstream->read(&unk_05);
		bstream->read(&unk_06);
		bstream->read(&unk_07);
		bstream->read(&unk_08);
	};
	void process(EventConnection* ps) 
	{
	};

	TNL_DECLARE_CLASS(CGShootReport);
};
#endif